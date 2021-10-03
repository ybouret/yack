

#include "yack/system/at-exit.hpp"
#include "yack/utest/run.hpp"


using namespace yack;

namespace
{
    static inline void proc1(void*)
    {
        std::cerr << "proc1" << std::endl;
    }
    
    static inline void proc2(void*)
    {
        std::cerr << "proc2" << std::endl;
    }

}

YACK_UTEST(at_exit)
{
    
    at_exit::perform(proc1, NULL,2);
    at_exit::perform(proc1, NULL,0);
    at_exit::perform(proc2, NULL,1);
    
}
YACK_UDONE()

