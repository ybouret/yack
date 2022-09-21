
//! \file
#ifndef YACK_ALGEBRA_TRIDIAG_INCLUDED
#define YACK_ALGEBRA_TRIDIAG_INCLUDED 1

#include "yack/math/api.hpp"
#include "yack/sequence/arrays.hpp"


namespace yack {

    namespace math {

        //______________________________________________________________________
        //
        //
        //! tridiagonal base class
        //
        //______________________________________________________________________
        class tridiag_
        {
        public:
            static const size_t  reserved = 4; //!< internal arrays
            const size_t         extraneous;   //!< extraneaous arrays
            virtual ~tridiag_() throw();       //!< cleanup

        protected:
            explicit tridiag_(const size_t extra) throw(); //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tridiag_);
        };

        //______________________________________________________________________
        //
        //! tridiagonal matrix interface and solver
        /**
         works for float, double, long double, complexes, mpq
         */
        //______________________________________________________________________
        template <typename T>
        class tridiag : public tridiag_, public arrays_of<T>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef arrays_of<T>                    tableaux;    //!< alias
            typedef typename tableaux::array_type   array_type;  //!< alias
            typedef typename scalar_for<T>::type    scalar_type; //!< alias
            using tableaux::next;
            using tableaux::mutual_size;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! destructor
            inline virtual ~tridiag() throw() {}

            //! setup with size=n, at least 4 arrays
            explicit tridiag(const size_t n, const size_t extra_arrays = 0):
            tridiag_(extra_arrays),
            tableaux(reserved+extra_arrays,n),
            a( next() ),
            b( next() ),
            c( next() ),
            g( next() )
            {
                assert(n==a.size());
                assert(n==b.size());
                assert(n==c.size());
                assert(n==g.size());
            }

            //__________________________________________________________________
            //
            //! get value at row i, column j
            //__________________________________________________________________
            inline T operator()(const size_t i, const size_t j) const throw()
            {
                assert(i>0); assert(i<=mutual_size());
                assert(j>0); assert(j<=mutual_size());
                if(i>j)
                {
                    if(1==i-j)
                    {
                        return a[i];
                    }
                    else
                    {
                        return 0;
                    }
                }
                else if( i<j )
                {
                    if(1==j-i)
                    {
                        return c[j-1];
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return b[i];
                }
            }

            //__________________________________________________________________
            //
            //! display
            //__________________________________________________________________
            inline friend std::ostream & operator<<( std::ostream &os, const tridiag &tri )
            {
                os << '[';
                const size_t n = tri.mutual_size();
                for(size_t i=1;i<=n;++i)
                {
                    for(size_t j=1;j<=n;++j)
                    {
                        os << ' ' << tri(i,j);
                    }
                    if(i<n) os << ';';
                }
                os << ']';
                return os;
            }


            //__________________________________________________________________
            //
            //! solve (*this)*u = r
            //__________________________________________________________________
            inline bool solve(writable<T> &u, const readable<T> &r )
            {
                assert(mutual_size()>0);
                assert(u.size()==mutual_size());
                assert(r.size()==mutual_size());

                // initialize
                const size_t n   = mutual_size(); assert(n>0);
                T            piv = b[1];
                {
                    const scalar_type apiv = abs_of(piv);
                    if(apiv<=0) return false;
                }

                // forward
                u[1]=r[1]/piv;
                for (size_t j=2;j<=n;j++)
                {
                    g[j]=c[j-1]/piv;
                    piv=b[j]-a[j]*g[j];
                    {
                        const scalar_type apiv = abs_of(piv);
                        if(apiv<=0) return false;
                    }
                    u[j]=(r[j]-a[j]*u[j-1])/piv;
                }

                // reverse
                for(size_t j=(n-1),jp=n;j>0;--j,--jp)
                {
                    u[j] -= g[jp]*u[jp];
                }
                return true;
            }

            //! target = (*this)*source
            inline void operator()( writable<T> &target, const readable<T> &source) const
            {
                assert(target.size()==mutual_size());
                assert(source.size()==mutual_size());
                const size_t n = mutual_size();
                switch(n)
                {
                    case 0: return;
                    case 1: target[1] = b[1] * source[1]; return;
                    default: {
                        assert(n>1);
                        const size_t nm1 = n-1;
                        target[1] = b[1] * source[1] + c[1] * source[2];
                        for(size_t i=nm1;i>1;--i)
                        {
                            target[i] = a[i] * source[i-1] + b[i] * source[i] + c[i] * source[i+1];
                        }
                        target[n] = a[n] * source[nm1] + b[n] * source[n];
                    } break;
                }
            }


            array_type &a; //!< a[2]..a[size]
            array_type &b; //!< b[1]..b[size]
            array_type &c; //!< c[1]..c[size-1]
        private:
            array_type &g;

            YACK_DISABLE_COPY_AND_ASSIGN(tridiag);

        protected:
            //! get array [0..extraneous-1]
            inline virtual const array_type & get_array(const size_t i) const throw()
            {
                assert(i<extraneous);
                const tableaux &tab = *this;
                return tab[i+reserved];
            }
        };


    }
}

#endif

