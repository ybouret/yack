#include "yack/fft/fft1d.hpp"
#include "yack/type/constants.hpp"
#include "yack/type/utils.hpp"
#include <cmath>


namespace yack
{
    
    volatile uint64_t fft1d::algo_ticks = 0;
    
    
    static const uint64_t one64 = 1;

    
    
#define YFFT(i) sin(YACK_PI_D/(one64<<(i)))
#define YREP()  \
YFFT( 0), YFFT( 1), YFFT( 2), YFFT( 3), YFFT( 4), YFFT( 5), YFFT( 6), YFFT( 7), \
YFFT( 8), YFFT( 9), YFFT(10), YFFT(11), YFFT(12), YFFT(13), YFFT(14), YFFT(15), \
YFFT(16), YFFT(17), YFFT(18), YFFT(19), YFFT(20), YFFT(21), YFFT(22), YFFT(23), \
YFFT(24), YFFT(25), YFFT(26), YFFT(27), YFFT(28), YFFT(29), YFFT(30), YFFT(31), \
YFFT(32), YFFT(33), YFFT(34), YFFT(35), YFFT(36), YFFT(37), YFFT(38), YFFT(39), \
YFFT(40), YFFT(41), YFFT(42), YFFT(43), YFFT(44), YFFT(45), YFFT(46), YFFT(47), \
YFFT(48), YFFT(49), YFFT(50), YFFT(51), YFFT(52), YFFT(53), YFFT(54), YFFT(55), \
YFFT(56), YFFT(57), YFFT(58), YFFT(59), YFFT(60), YFFT(61), YFFT(62), YFFT(63)
    
    const double fft1d::pos_sine[64] =
    {
        YREP()
    };
    
#undef  YFFT
#define YFFT(i) -sin(YACK_PI_D/(one64<<(i)))
    
    const double fft1d::neg_sine[64] =
    {
        YREP()
    };
    
#undef  YFFT
#define YFFT(i) -twice(squared(sin(YACK_PI_D/(one64<<(i)))))
    
    const double fft1d::twpr[64] =
    {
        YREP()
    };
}

