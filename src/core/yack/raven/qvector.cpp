
#include "yack/raven/qvector.hpp"
#include "yack/apex/kernel.hpp"

namespace yack
{
    namespace raven
    {
        qvector:: ~qvector() throw()
        {
        }

        qvector:: qvector(apz *ptr, const size_t num) :
        qmetrics(num),
        coeff(ptr),
        norm2(0)
        {
            assert(NULL!=coeff);
            --coeff;
        }


        std::ostream & operator<<(std::ostream &os, const qvector &self)
        {
            const readable<apz> &arr = self;
            os << "|" << arr << "|^2 = " << self.norm2;
            return os;
        }

        size_t qvector:: size() const throw() { return dimension; }

        const apz & qvector:: operator[](const size_t i) const throw() {
            assert(i>=1); assert(i<=dimension);
            return coeff[i];
        }

        void qvector:: normalize()
        {
            thin_array<apz> self( coerce(&coeff[1]), dimension);
            apk::definite(self,coerce(norm2));
        }


        void qvector:: sub(writable<apq>       &u_k,
                           const readable<apz> &v_k)
        {
            const readable<apz> &u_j = *this;
            assert(norm2.size());
            assert(dimension==u_j.size());
            assert(dimension==u_k.size());
            assert(dimension==v_k.size());

            // dot product v_k u_j
            const size_t nd = dimension;
            size_t       ii = nd;
            apz          uv = v_k[ii] * u_j[ii];
            for(--ii;ii>0;--ii) uv += v_k[ii] * u_j[ii];

            // scaling coefficient
            const apq  cof(uv,norm2);
            bool       gtz = false;
            for(size_t i=nd;i>0;--i)
            {
                if( (u_k[i] -= cof * u_j[i]).num.n.size() > 0 ) gtz = true;
            }

            //return gtz;

        }

    }

}

