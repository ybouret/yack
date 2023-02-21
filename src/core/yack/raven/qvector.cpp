
#include "yack/raven/qvector.hpp"
#include "yack/apex/alga.hpp"

namespace yack
{
    namespace raven
    {
        qvector:: ~qvector() noexcept
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

        void qvector:: ldz() noexcept
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

        size_t qvector:: size() const noexcept { return dimension; }

        const apz & qvector:: operator[](const size_t i) const noexcept {
            assert(i>=1); assert(i<=dimension);
            return coeff[i];
        }


        
        bool qvector:: appointed(const readable<apq> &u_k)
        {
            static const size_t one(1);
            assert(u_k.size()==dimension);

            thin_array<apz> arr( &coeff[1], dimension);
            const apn       lcm = alga::lcm(u_k,one,dimension);
            bool            gtz = false;

            for(size_t i=dimension;i>0;--i)
            {
                const apq q = u_k[i] * lcm; assert(1==q.den);
                if( (arr[i] = q.num).s != __zero__ ) gtz = true;
            }

            if(gtz)
            {
                alga::definite(arr,coerce(norm2));
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
            // use self as u_j in G-S algo
            const readable<apz> &u_j = *this;
            assert(norm2.size());
            assert(dimension==u_j.size());
            assert(dimension==u_k.size());
            assert(dimension==v_k.size());

            // dot product <v_k|u_j>
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


        int qvector:: compare(const qvector &lhs, const qvector &rhs) noexcept
        {
            assert(lhs.size() == rhs.size() );
            return comparison::lexicographic(&lhs[1], &rhs[1], lhs.size() );
        }

    }

}

