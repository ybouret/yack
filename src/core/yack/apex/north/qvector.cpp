
#include "yack/apex/north/qvector.hpp"

namespace yack
{

    namespace north
    {


        qvector:: qvector(apz *ptr, const size_t num) :
        qmetrics(num),
        coeff(ptr),
        norm2(0)
        {
            assert( yack_good(ptr,num) );
            --coeff;
        }

        qvector:: ~qvector() throw() {}

        size_t qvector:: size() const throw() { return dimension; }

        qvector::const_type & qvector:: operator[](size_t indx) const throw()
        {
            assert(indx>=1); assert(indx<=dimension); return coeff[indx];
        }

        void qvector:: xch(qvector &other) throw()
        {
            assert(dimension==other.dimension);
            coerce(norm2).xch(coerce(other.norm2));
            for(size_t i=dimension;i>0;--i)
                coerce(coeff[i]).xch(coerce(other.coeff[i]) );
        }

        apz qvector:: dot(const readable<apz> &v) const
        {
            assert(v.size()==dimension);
            apz sum = 0;
            const readable<apz> &self = *this;
            for(size_t i=dimension;i>0;--i)
            {
                sum += v[i] * self[i];
            }
            return sum;
            //return apq(sum,norm2);
        }

        std::ostream & operator<<( std::ostream &os, const qvector &self )
        {
            os << static_cast<const readable<apz> &>(self) << '#' << self.norm2;
            return os;
        }

        bool qvector:: grow(writable<apq>           &u_k,
                            const readable<apz>     &v_k,
                            const readable<qvector> &U)
        {
            assert(u_k.size()==v_k.size());
            const size_t dim = u_k.size();
            const size_t sub = U.size();
            for(size_t j=1;j<=sub;++j)
            {
                const qvector &u_j = U[j]; assert(u_j.dimension==dim);
                const apq      cof(u_j.dot(v_k),u_j.norm2);
                bool           gtz = false;
                for(size_t i=dim;i>0;--i)
                {
                    if( (u_k[i] -= cof * u_j[i]).num.n.size() != 0 ) gtz=true;
                }
                //assert(are_orthogonal(u_k,u_j));
                if(!gtz) return false;
            }

            return true;
        }
    }

}

