//! \file

#ifndef YACK_UTEST_RUN_INCLUDED
#define YACK_UTEST_RUN_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/check/crc32.h"
#include "yack/type/mswap.hpp"
#include "yack/check/expression.hpp"
#include "yack/randomized/rand.hpp"
#include "yack/randomized/shuffle.hpp"

#include <iostream>
#include <iomanip>
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
        return yack_crc32(  out_of_reach::address(addr), size*sizeof(T) );
    }

    
    //__________________________________________________________________________
    //
    //
    //! utility to display sizeof
    //
    //__________________________________________________________________________
    struct utest_sizeof
    {
        static size_t width; //!< for alignment
        static void   print(const char *name, const uint64_t size); //!< print
        static void   field(const char *name, const uint64_t size); //!< output field
    };
    
    //! to display aligned sizeof
#define YACK_SIZEOF(CLASS) utest_sizeof::print( #CLASS, sizeof(CLASS) )

    //! to display aligned field
#define YACK_OUTPUT(FIELD) utest_sizeof::field( #FIELD, FIELD )


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
