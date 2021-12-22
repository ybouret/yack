
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


        bits:: bits(const uint32_t umax) throw() :
        metrics(umax)
        {

        }

        double bits:: operator()(void) throw()
        {
            return unit_<double>( next32() );
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

        // units

        template <> double bits:: to<double>() throw()
        {
            return unit_<double>(next32());
        }


        template <> float bits:: to<float>() throw()
        {
            return unit_<float>( next32() );
        }
        
        template <> long double bits:: to<long double>() throw()
        {
            return unit_<long double>( next32() );
        }

        template <> uint8_t bits:: to<uint8_t>() throw()
        {
            const double x = (*this)();
            return uint8_t( floor(255.0*x+0.5) );
        }

        template <> uint16_t bits:: to<uint16_t>() throw()
        {
            const double x = (*this)();
            return uint16_t( floor(65535.0*x+0.5) );
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

        // symms
        template <> float bits:: symm<float>() throw()
        {
            return symm_<float>( next32() );
        }

        template <> double bits:: symm<double>() throw()
        {
            return symm_<double>( next32() );
        }

        template <> long double bits:: symm<long double>() throw()
        {
            return symm_<long double>( next32() );
        }

        template <> int8_t bits:: symm<int8_t>() throw()
        {
            return int8_t( to<uint8_t>() );
        }

        template <> int16_t bits:: symm<int16_t>() throw()
        {
            return int16_t( to<uint16_t>() );
        }

        template <> int32_t bits:: symm<int32_t>() throw()
        {
            return int32_t( to<uint32_t>() );
        }

        template <> int64_t bits:: symm<int64_t>() throw()
        {
            return int64_t( to<uint64_t>() );
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
