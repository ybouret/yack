
#include "yack/info/entropy.hpp"
#include <cstring>
#include <cmath>

namespace yack
{

    namespace information
    {

        entropy:: ~entropy() noexcept
        {
            reset();
        }

        entropy:: entropy() noexcept :
        total(0),
        count()
        {
            reset();
        }

        void entropy:: reset() noexcept
        {
            total = 0;
            memset(count,0,sizeof(count));
        }

        entropy & entropy:: operator<<(const uint8_t byte) noexcept
        {
            ++total;
            ++count[byte];
            return *this;
        }

        entropy & entropy:: operator()(const void *addr,size_t size) noexcept
        {
            assert(!(NULL==addr&&size>0));
            const uint8_t *q = static_cast<const uint8_t *>(addr);
            while(size-- > 0)
            {
                (*this) << *(q++);
            }
            return *this;
        }

        double entropy:: operator()(void) const noexcept
        {
            double sum = 0;
            for(size_t i=0;i<256;++i)
            {
                size_t n = count[i];
                if(n)
                {
                    assert(total>=n);
                    const double p = double(n)/total;
                    sum += p * log(p);
                }
            }
            return -sum;
        }

        double entropy:: of(const void *addr, const size_t size) noexcept
        {
            entropy self;
            return self(addr,size)();
        }



    }

}
