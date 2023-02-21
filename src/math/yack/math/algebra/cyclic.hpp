//! \file
#ifndef YACK_ALGEBRA_CYCLIC_INCLUDED
#define YACK_ALGEBRA_CYCLIC_INCLUDED 1

#include "yack/math/algebra/tridiag.hpp"
#include "yack/math/algebra/crout.hpp"
#include "yack/type/args.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack {

    namespace math {

        //______________________________________________________________________
        //
        //
        //! cyclic size check
        //
        //______________________________________________________________________
        class cyclic_
        {
        public:
            virtual ~cyclic_() noexcept;     //!< cleanup
        protected:
            explicit cyclic_(const size_t); //!< throw below 3

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(cyclic_);
        };

        //______________________________________________________________________
        //
        //
        //! at least 3-cyclic system, using Woodbury
        //
        //______________________________________________________________________
        template <typename T>
        class cyclic : public cyclic_,  public tridiag<T>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            YACK_DECL_ARGS(T,type);                                 //!< alias
            typedef typename arrays_of<T>::array_type  array_type;  //!< alias
            typedef typename scalar_for<T>::type       scalar_type; //!< alias
            static const  size_t                       space = 2;   //!< number of extra values


            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! cleanup
            inline virtual ~cyclic() noexcept {}

            //! allocate resources
            inline cyclic(const size_t n) :
            cyclic_(n),
            tridiag<T>(n,1),
            LU(n),
            HVx( this->next() ),
            U(space,n),
            V(space,n),
            Z(space,n),
            H(space,space),
            one(1)
            {
                U[1][1] = one;
                U[2][n] = one;
            }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! set cyclic A,B,C
            inline void set( param_type A, param_type B, param_type C )
            {
                assert(this->mutual_size()>=3);
                for(size_t i=this->mutual_size();i>0;--i)
                {
                    this->a[i] = A;
                    this->b[i] = B;
                    this->c[i] = C;
                }
            }


            //! solve (*this)*x = r
            inline bool solve( writable<T> &x, const readable<T> &r )
            {
                assert( this->mutual_size() == x.size() );
                assert( this->mutual_size() == r.size() );
                assert( this->mutual_size() >= 3        );

                // setup V
                const size_t n = this->mutual_size();
                V[1][n] = this->a[1];
                V[2][1] = this->c[n];


                // compute Z
                for(size_t i=space;i>0;--i)
                {
                    writable<T> &Zi = Z[i];
                    tridiag<T>::solve(Zi,U[i]);
                }

                // compute H = inv(1+V'*Z)
                for(size_t i=space;i>0;--i)
                {
                    writable<T>       &Hi = H[i];
                    const readable<T> &Vi = V[i];
                    for(size_t j=space;j>0;--j)
                    {
                        type ans = 0;
                        {
                            const readable<T> &Zj = Z[j];
                            for(size_t k=n;k>0;--k)
                            {
                                ans += Vi[k] * Zj[k];
                            }
                        }
                        Hi[j] = ans;
                    }
                    Hi[i] += one;
                }

                if( !LU.build(H) )
                {
                    return false;
                }

                // raw solution in x
                tridiag<T>::solve(x,r);

                // compute H * V' * x
                for(size_t i=space;i>0;--i)
                {
                    const readable<T> &Vi = V[i];
                    type ans = 0;
                    for(size_t k=n;k>0;--k)
                    {
                        ans += Vi[k] * x[k];
                    }
                    HVx[i] = ans;
                }
                LU.solve(H,HVx);

                // substact Z * (H * V' * x )
                for(size_t k=n;k>0;--k)
                {
                    for(size_t j=space;j>0;--j)
                    {
                        x[k] -= Z[j][k] * HVx[j];
                    }
                }
                return true;
            }

            //! get value at row i, column j
            inline T operator()(const size_t i, const size_t j) const noexcept
            {
                assert(i>0); assert(i<=this->mutual_size());
                assert(j>0); assert(j<=this->mutual_size());
                const size_t n = this->mutual_size();
                if(i==1)
                {
                    //__________________________________________________________
                    //
                    // first row
                    //__________________________________________________________
                    if(j==1)
                    {
                        return this->b[1];
                    }
                    else if(j==2)
                    {
                        return this->c[1];
                    }
                    else if(j==n)
                    {
                        return this->a[1];
                    }
                    else
                    {
                        return 0;
                    }
                }
                else if(i==n)
                {
                    //__________________________________________________________
                    //
                    // last row
                    //__________________________________________________________
                    if(j==1)
                    {
                        return this->c[n];
                    }
                    else if(j==n)
                    {
                        return this->b[n];
                    }
                    else if(j==n-1)
                    {
                        return this->a[n];
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    //__________________________________________________________
                    //
                    // bulk
                    //__________________________________________________________
                    if(i>j)
                    {
                        if(1==i-j)
                        {
                            return this->a[i];
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
                            return this->c[j-1];
                        }
                        else
                        {
                            return 0;
                        }
                    }
                    else
                    {
                        return this->b[i];
                    }
                }

            }

            //! display
            inline friend std::ostream & operator<<(std::ostream &os, const cyclic &cc)
            {
                os << '[';
                const size_t n = cc.mutual_size();
                for(size_t i=1;i<=n;++i)
                {
                    for(size_t j=1;j<=n;++j)
                    {
                        os << ' ' << cc(i,j);
                    }
                    if(i<n) os << ';';
                }
                os << ']';
                return os;
            }

            //! target = (*this)*source
            inline void operator()( writable<T> &target, const readable<T> &source ) const
            {
                assert(this->mutual_size()>=3);
                assert(target.size()==this->mutual_size());
                assert(source.size()==this->mutual_size());
                const size_t n = this->mutual_size();

                // first row
                target[1] = this->b[1] * source[1] + this->c[1] * source[2] + this->a[1] * source[n];

                // last row
                target[n] = this->a[n] * source[n-1] + this->b[n] * source[n] + this->c[n] * source[1];

                // bulk
                for(size_t i=2;i<n;++i)
                {
                    target[i] = this->a[i] * source[i-1] + this->b[i] * source[i] + this->c[i] * source[i+1];
                }

            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            crout<T>      LU; //!< helper
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(cyclic);
            array_type &HVx;
            matrix<T>   U;   //!< matrix of column position, made of ones
            matrix<T>   V;   //!< matrix of rows position+values
            matrix<T>   Z;   //!< local problems
            matrix<T>   H;   //!< dispatch matrix
            const_type  one; //!< precomputed 1

          
        };

    }
}

#endif


