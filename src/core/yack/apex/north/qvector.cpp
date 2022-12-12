
#include "yack/apex/north/qvector.hpp"

namespace yack
{

    namespace north
    {

        bool qvector:: are_orthogonal(const readable<apq> &lhs,
                                      const readable<apq> &rhs)
        {
            assert( lhs.size() == rhs.size() );
            apq sum = 0;
            for(size_t i=lhs.size();i>0;--i) sum += lhs[i] * rhs[i];
            return 0 == sum;
        }


        qvector:: qvector(apq *ptr, const size_t num) :
        qmetrics(num),
        coeff(ptr),
        norm2(0)
        {
            assert( yack_good(ptr,num) );
            --coeff;
        }

        qvector:: ~qvector() throw() {}

        size_t qvector:: size() const throw() { return dimension; }

        const apq & qvector:: operator[](size_t indx) const throw()
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

        apq qvector:: weight(const readable<apq> &v) const
        {
            assert(v.size()==dimension);
            apq sum = 0;
            const readable<apq> &self = *this;
            for(size_t i=dimension;i>0;--i)
            {
                sum += v[i] * self[i];
            }
            return sum/norm2;
        }

        std::ostream & operator<<( std::ostream &os, const qvector &self )
        {
            os << static_cast<const readable<apq> &>(self) << '#' << self.norm2;
            return os;
        }

        bool qvector:: grow(writable<apq>           &u_k,
                            const readable<apq>     &v_k,
                            const readable<qvector> &U)
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
                assert(are_orthogonal(u_k,u_j));
                if(sum<=0) return false;
            }

            return true;
        }

    }

}

