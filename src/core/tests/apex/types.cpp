#include "yack/apex/types.hpp"
#include "yack/utest/run.hpp"
#include "yack/memory/allocator/dyadic.hpp"


using namespace yack;

namespace
{

    template <typename T> static inline
    void do_test_mem(uprng &ran)
    {
        int         arr_exp2 =  5;
        int         blk_exp2 = -1;
        T          *arr      = apex::cull::field_acquire<T>(arr_exp2,blk_exp2);
        size_t      arr_size = 1 << arr_exp2;

        std::cerr << "arr_exp2=" << arr_exp2 << " => bkl_exp2=" << blk_exp2 << " size=" << arr_size << ", bytes=" << (1<<blk_exp2) << std::endl;
        ran.fillnz(arr,arr_size*sizeof(T));

        apex::cull::field_release(arr,arr_exp2,blk_exp2);

    }
}

YACK_UTEST(apex_types)
{
    uprng ran;
    std::cerr << "core_bits=" << apex::cull::core_bits << std::endl;
    std::cerr << "work_bits=" << apex::cull::work_bits << std::endl;

    YACK_CHECK(sizeof(apex::cull::core_type)==apex::cull::core_size);
    do_test_mem<uint8_t>(ran);
    do_test_mem<uint16_t>(ran);
    do_test_mem<uint32_t>(ran);
    do_test_mem<uint64_t>(ran);

    memory::dyadic::instance().display();

}
YACK_UDONE()
