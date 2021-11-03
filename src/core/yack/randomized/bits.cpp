
#include "yack/randomized/bits.hpp"
#include "yack/arith/align.hpp"
#include <cmath>

namespace yack
{

    namespace randomized
    {

        bits:: ~bits() throw()
        {
        }


        bits:: bits(const size_t num_bits) throw() :
        count(num_bits),
        bytes(YACK_ALIGN_ON(8,count) >> 3)
        {

        }

        size_t bits:: leq(const size_t value) throw()
        {
            bits &ran = *this;
            return floor( value * ran() + 0.5);
        }

        unit_t bits::in(const unit_t a, const unit_t b) throw()
        {
            bits        &ran = *this;
            const unit_t res = a + unit_t( floor( (b-a) * ran() + 0.5 ) );
            assert(res>=a); assert(res<=b);
            return res;
        }

        bool bits:: choice() throw()
        {
            return (*this)() <= 0.5;
        }


        template <> double bits:: to<double>() throw()
        {
            return (*this)();
        }


        template <> float bits:: to<float>() throw()
        {
            return (*this)();
        }

        template <> uint8_t bits:: to<uint8_t>() throw()
        {
            const double x = (*this)();
            return uint8_t( floor(255*x+0.5) );
        }

        template <> uint16_t bits:: to<uint16_t>() throw()
        {
            const double x = (*this)();
            return uint16_t( floor(65535*x+0.5) );
        }

        template <> uint32_t bits:: to<uint32_t>() throw()
        {
            const double x = (*this)();
            return uint32_t( floor(4294967295.0*x+0.5) );
        }

        template <> uint64_t bits:: to<uint64_t>() throw()
        {
            const double   x = (*this)();
            const double   y = (*this)();
            const uint64_t hi = uint32_t( floor(4294967295.0*x+0.5) );
            const uint64_t lo = uint32_t( floor(4294967295.0*y+0.5) );
            return (hi<<32) | lo;
        }


        void bits:: fill(void *addr, size_t size) throw()
        {
            uint8_t *p = static_cast<uint8_t*>(addr);
            while(size-- >0)
            {
                *(p++) = uint8_t(1+leq(254));
            }
        }


    }

}
