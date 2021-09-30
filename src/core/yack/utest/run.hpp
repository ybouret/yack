//! \file

#ifndef YACK_UTEST_RUN_INCLUDED
#define YACK_UTEST_RUN_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/check/crc32.h"
#include "yack/type/mswap.hpp"
#include "yack/check/expression.hpp"

#include <iostream>
#include <cmath>

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! utility to check memory blocks
    //
    //__________________________________________________________________________
    template <typename T> inline
    uint32_t ucrc(const T *addr, const size_t size) throw()
    {
        assert( !(NULL==addr&&0<size) );
        return yack_crc32( yack_out_of_reach_constant(addr), size*sizeof(T) );
    }

    //__________________________________________________________________________
    //
    //
    //! utility to generate quick uniform pseudo random number
    //
    //__________________________________________________________________________
    class uprng
    {
    public:
        explicit uprng() throw(); //!< call srand( time(NULL) )
        virtual ~uprng() throw(); //!< cleanup

        //! in ]0:1[
        double operator()(void)  throw();

        //! in [0:M]
        template <typename T> inline
        T leq(const T M) throw()
        {
            uprng &self = *this;
            return T( floor( double(M) * self() + 0.5 ) );
        }

        //! Knuth shuffle
        template <typename T> inline
        void shuffle(T *addr, const size_t size) throw()
        {
            if(size>1)
            {
                for(size_t i=size-1;i>0;--i)
                {
                    const size_t j = leq<size_t>(i);
                    assert(j<=i);
                    mswap(addr[i],addr[j]);
                }
            }
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(uprng);

    };




//! start a new test
#define YACK_UTEST(NAME)                                               \
/**/  int yack_test_##NAME(int argc, const char **argv) throw() {      \
/**/  const char *program = argv[0]; (void) argc; (void) program; try


//! leave the test
#define YACK_UDONE()                                               \
/**/    catch(const yack::exception &e) {                          \
/**/      e.show(std::cerr); return -1; }                          \
/**/    catch(const std::exception  &e) {                          \
/**/      std::cerr << " *** std::exception" << std::endl;         \
/**/      std::cerr << " *** " << e.what()   << std::endl; }       \
/**/    catch(...) {                                               \
/**/      std::cerr << " *** Unhandled Exception" << std::endl; }  \
/**/    return 0;                                                  \
/**/  }

}
#endif
