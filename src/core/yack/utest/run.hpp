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
        return yack_crc32(  out_of_reach::address(addr), size*sizeof(T) );
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

        //! true/false at 50/50
        inline bool choice() throw()
        {
            return (*this)() <= 0.5 ? true : false;
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

        //! card-desk shuffle for lists
        template <typename LIST> inline
        void shuffle(LIST &l) throw()
        {
            LIST tmp;
            while(l.size)
            {
                typename LIST::node_type *node=l.pop_back();
                if(choice()) tmp.push_back(node); else tmp.push_front(node);
            }
            tmp.swap_with(l);
        }
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(uprng);

    };

    //__________________________________________________________________________
    //
    //
    //! utility to display sizeof
    //
    //__________________________________________________________________________
    struct utest_sizeof
    {
        static size_t width; //!< for alignment
        static void   print(const char *name, const unsigned size); //!< print
    };
    
    //! to display aligned sizeof
#define YACK_SIZEOF(CLASS) utest_sizeof::print( #CLASS, sizeof(CLASS) )


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
