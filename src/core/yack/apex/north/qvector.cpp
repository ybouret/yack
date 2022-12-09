
#include "yack/apex/north/qvector.hpp"

namespace yack
{

    namespace north
    {
        template <>
        apq qvector<apq>:: weight(const readable<apq> &v) const
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

        template <>
        apq qvector<int64_t>:: weight(const readable<apq> &v) const
        {
            assert(v.size()==dimension);
            apq sum = 0;
            const readable<int64_t> &self = *this;
            for(size_t i=dimension;i>0;--i)
            {
                const apq rhs(self[i]);
                sum += v[i] * rhs;
            }
            return sum/norm2;
        }

    }

}

