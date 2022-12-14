
#include "yack/apex/north/qvector.hpp"
#include "yack/ios/encoder.hpp"

namespace yack
{

    namespace north
    {

        const char *qvector:: class_uid() const throw()
        {
            return "north::qvector";
        }

        size_t qvector:: serialize(ios::ostream &os) const
        {
            const readable<type> &self = *this;
            size_t res = ios::encoder::serialize(os,dimension);
            for(size_t i=1;i<=dimension;++i)
            {
                res += self[i].serialize(os);
            }
            return res;
        }

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

        bool qvector:: perp(const readable<apq> &lhs, const readable<apz> &rhs)
        {
            assert(lhs.size()==rhs.size());
            apq sum = 0;

            for(size_t i=lhs.size();i>0;--i)
            {
                sum += rhs[i] * lhs[i];
            }

            return sum.num.s == __zero__;
        }

        bool qvector:: neqz(const readable<apq> &u) throw()
        {
            for(size_t i=u.size();i>0;--i)
            {
                if(u[i].num.s != __zero__) return  true;
            }
            return false;
        }

        bool qvector:: grow(writable<apq>           &u_k,
                            const readable<apz>     &v_k,
                            const readable<qvector> &U)
        {
            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            assert(u_k.size()==v_k.size());
            const size_t dim = u_k.size();
            const size_t sub = U.size();

            //------------------------------------------------------------------
            //
            // starting from u_k = v_k, substract all components
            // on previous vectors
            //
            //------------------------------------------------------------------
            for(size_t j=sub;j>0;--j)
            {
                const qvector &u_j = U[j];                  assert(u_j.dimension==dim);
                const apq      cof(u_j.dot(v_k),u_j.norm2); // projection coefficient
                bool           gtz = false;
                for(size_t i=dim;i>0;--i)
                {
                    // upgrade vector, check is not null at once
                    if( (u_k[i] -= cof * u_j[i]).num.s != __zero__ ) gtz=true;
                }
                assert(perp(u_k,u_j));
                if(!gtz) return false;
            }
            
            assert(neqz(u_k));
            return true;
        }
    }

}

