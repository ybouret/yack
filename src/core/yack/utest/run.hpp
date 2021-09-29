//! \file

#ifndef YACK_UTEST_RUN_INCLUDED
#define YACK_UTEST_RUN_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/check/crc32.h"
#include "yack/type/mswap.hpp"

#include <iostream>
#include <cmath>

namespace yack
{

    template <typename T> inline
    uint32_t ucrc(const T *addr, const size_t size) throw()
    {
        assert( !(NULL==addr&&0<size) );
        return yack_crc32( yack_out_of_reach_constant(addr), size*sizeof(T) );
    }

    class uprng
    {
    public:
        explicit uprng() throw();
        virtual ~uprng() throw();

        double operator()(void)  throw();

        template <typename T> inline
        void shuffle(T *addr, const size_t size) throw()
        {
            uprng &self = *this;
            if(size>1)
            {
                for(size_t i=size-1;i>0;--i)
                {
                    const size_t j = size_t( floor( double(i) * self() + 0.5 ) );
                    assert(j<=i);
                    mswap(addr[i],addr[j]);
                }
            }
        }



    private:
        YACK_DISABLE_COPY_AND_ASSIGN(uprng);

    };




//! start a new test
#define YACK_UTEST(NAME) \
/**/  int yack_test_##NAME(int argc, const char **argv) throw() {\
/**/  const char *program = argv[0]; (void) argc; (void) program; try


//! leave the test
#define YACK_UDONE() \
/**/    catch(...) {}\
/**/    return 0;    \
/**/  }

}
#endif
