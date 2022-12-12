
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

        //______________________________________________________________________
        //
        //
        //! component of an orthogonal family
        //
        //______________________________________________________________________
        template <typename T> class qvector : public qmetrics, public readable<T>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef classify<T>                      class_ops;
            typedef typename class_ops::l2_type      l2_type;    //!< alias
            typedef typename readable<T>::type       type;       //!< alias
            typedef typename readable<T>::const_type const_type; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup from initialized ptr[num]
            inline explicit qvector(type *ptr, const size_t num) :
            qmetrics(num),
            coeff(ptr),
            norm2(0)
            {
                assert( yack_good(ptr,num) );
                --coeff;
            }

            //! cleanup assigned space
            inline virtual ~qvector() throw() { }

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

            inline void xch(qvector &other) throw()
            {
                assert(dimension==other.dimension);
                class_ops:: xch( coerce(norm2), coerce(other.norm2) );
                for(size_t i=dimension;i>0;--i)
                    class_ops::xch( coerce(coeff[i]), coerce(other.coeff[i]) );
            }


            //! compute Gram-Schmidt weight = <coef|v>/nrm2
            apq  weight(const readable<apq> &v) const;


            //__________________________________________________________________
            //
            //! construct in u_k the orthogonal version of v_k
            /**
             using Gram-Schmidt direct algorithm
             */
            //__________________________________________________________________
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

            //__________________________________________________________________
            //
            //! human friendly display
            //__________________________________________________________________
            inline friend std::ostream & operator<<( std::ostream &os, const qvector &self )
            {
                os << static_cast<const readable<T> &>(self) << '#' << self.norm2;
                return os;
            }


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(qvector);
            type         *coeff;

        public:
            const l2_type norm2; //!< current squared norm

            
        };
    }

}

#endif
