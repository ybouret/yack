#include "yack/apex/types.hpp"
#include "yack/apex/n/alloc.hpp"
#include "yack/utest/run.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include <cstdio>

using namespace yack;

namespace
{

    template <typename T> static inline
    void do_test_mem(uprng &ran)
    {
        int         arr_exp2 =  5;
        int         blk_exp2 = -1;
        T          *arr      = apex::alloc::field_acquire<T>(arr_exp2,blk_exp2);
        size_t      arr_size = 1 << arr_exp2;

        std::cerr << "arr_exp2=" << arr_exp2 << " => bkl_exp2=" << blk_exp2 << " size=" << arr_size << ", bytes=" << (1<<blk_exp2) << std::endl;
        ran.fillnz(arr,arr_size*sizeof(T));

        apex::alloc::field_release(arr,arr_exp2,blk_exp2);

    }
}

YACK_UTEST(apex_types)
{
    uprng ran;
    

    do_test_mem<uint8_t>(ran);
    do_test_mem<uint16_t>(ran);
    do_test_mem<uint32_t>(ran);
    do_test_mem<uint64_t>(ran);

    memory::dyadic::instance().display();

    if(false)
    {
        std::cerr.flush();
        unsigned h=0;
        for(size_t i=0;i<16;++i)
        {
            for(size_t j=0;j<16;++j,++h)
            {
                fprintf(stdout," \"%02x\",",h);
            }
            fprintf(stdout,"\n");
        }
        fprintf(stdout,"\n");

        h=0;
        for(size_t i=0;i<16;++i)
        {
            for(size_t j=0;j<16;++j,++h)
            {
                fprintf(stdout," \"%02X\",",h);
            }
            fprintf(stdout,"\n");
        }
        fprintf(stdout,"\n");
    }
    
  
    
}
YACK_UDONE()
