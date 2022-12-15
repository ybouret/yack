
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

        void qvector:: hard_copy(const qvector &other)
        {
            assert(dimension==other.dimension);
            coerce(norm2) = other.norm2;
            for(size_t i=dimension;i>0;--i)
            {
                coeff[i] = other.coeff[i];
            }
        }

        void qvector:: ldz() throw()
        {
            coerce(norm2).ldz();
            for(size_t i=dimension;i>0;--i)
                coeff[i].ldz();
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

#if 0
        void qvector:: normalize()
        {
            thin_array<apz> self( &coeff[1], dimension);
            apk::definite(self,coerce(norm2));
        }
#endif
        
        bool qvector:: appointed(const readable<apq> &u_k)
        {
            assert(u_k.size()==dimension);
            thin_array<apz> arr( &coeff[1], dimension);
            const apn       lcm = apk::lcm(u_k, size_t(1), dimension);
            bool            gtz = false;

            for(size_t i=dimension;i>0;--i)
            {
                const apq q = u_k[i] * lcm;
                if( (arr[i] = q.num).s != __zero__ ) gtz = true;
            }

            if(gtz)
            {
                apk::definite(arr,coerce(norm2));
                return true;
            }
            else
            {
                coerce(norm2).ldz();
                return false;
            }
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
            for(size_t i=nd;i>0;--i)
            {
                u_k[i] -= cof * u_j[i];
            }


        }

    }

}

