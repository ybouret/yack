
//! \file

#ifndef YACK_APEX_NORTH_QVECTOR_INCLUDED
#define YACK_APEX_NORTH_QVECTOR_INCLUDED 1

#include "yack/apex/north/constellation.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/type/destruct.hpp"

namespace yack
{

    namespace north
    {

        
        template <typename T> class qvector : public readable<T>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef typename classify<T>::l2_type    l2_type;    //!< alias
            typedef typename readable<T>::type       type;       //!< alias
            typedef typename readable<T>::const_type const_type; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup concerned area
            inline explicit qvector(type *ptr, const size_t num) :
            dimension(num), coeff(ptr), norm2(0)
            {
                assert( yack_good(ptr,num) );
                --coeff;
                init();
            }

            //! cleanup assigned space
            inline virtual ~qvector() throw() { quit(dimension); }

            //__________________________________________________________________
            //
            // readable interface
            //__________________________________________________________________
            inline virtual size_t size() const throw() { return dimension; } //!< size = matrix.dimension

            inline virtual const_type & operator[](size_t indx) const throw()
            {
                assert(indx>=1); assert(indx<=dimension); return coeff[indx];
            }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________


            //! compute Gram-Schmidt weight = <coef|v>/nrm2
            apq  weight(const readable<apq> &v) const;



            static inline
            bool grow(writable<apq>                &u_k,
                      const readable<apq>          &v_k,
                      const readable< qvector<T> > &U)
            {
                assert(u_k.size()==v_k.size());
                const size_t dim = u_k.size();
                const size_t sub = U.size();
                for(size_t j=1;j<=sub;++j)
                {
                    const qvector &u_j = U[j]; assert(u_j.dimension==dim);
                    const apq      cof = u_j.weight(v_k);
                    apn            sum = 0;
                    for(size_t i=dim;i>0;--i)
                    {
                        sum += (u_k[i] -= cof * u_j[i]).num.n;
                    }
                    assert(constellation::are_orthogonal(u_k,u_j));
                    if(sum<=0) return false;
                }

                return true;
            }

            inline friend std::ostream & operator<<( std::ostream &os, const qvector &self )
            {
                os << static_cast<const readable<T> &>(self) << '#' << self.norm2;
                return os;
            }

            static inline bool eq(const qvector &lhs, const qvector &rhs)
            {
                assert(lhs.size()==rhs.size());
                const size_t n=lhs.size();
                for(size_t i=n;i>0;--i)
                {
                    if(lhs[i]!=rhs[i]) return false;
                }

                return true;
            }



            //! lexicographic comparison
            static inline int compare(const qvector &lhs, const qvector &rhs) throw()
            {
                assert(lhs.size()==rhs.size());
                const size_t n=lhs.size();
                for(size_t i=1;i<=n;++i)
                {
                    switch(__sign::of(lhs[i],rhs[i]))
                    {
                        case __zero__: continue;
                        case negative: return -1;
                        case positive: return  1;
                    }
                }
                return 0;
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t dimension;
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(qvector);
            type         *coeff;

        public:
            const l2_type norm2;

        private:
            inline void quit(size_t done) throw()
            {
                type  *addr = coeff+1;
                while(done-- > 0) out_of_reach::naught( destructed(addr+done) );
            }

            inline void init() {
                size_t done = 0;
                type  *addr = coeff+1; assert(out_of_reach::is0(addr,dimension*sizeof(type)));
                try {
                    while(done<dimension) {
                        new(addr+done) type();
                        ++done;
                    }
                }
                catch(...) { quit(done); throw; }
            }
        };
    }

}

#endif
