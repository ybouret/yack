
#include "yack/color/yuv.hpp"
#include "yack/color/unit-real.hpp"

#define real_t float
#include "yuv.hxx"

#undef real_t
#define real_t double
#include "yuv.hxx"

#undef real_t
#define real_t long double
#include "yuv.hxx"
