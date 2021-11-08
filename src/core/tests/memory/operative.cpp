
#include "yack/memory/operative.hpp"
#include "yack/utest/run.hpp"
#include "yack/apex.hpp"
#include "yack/arith/align.hpp"
#include "yack/type/destruct.hpp"

using namespace yack;

namespace
{
    static inline void start(void *addr)
    {
        new (addr) apq();
    }

    static inline void clear(void *addr) throw()
    {
        destruct( static_cast<apq *>(addr) );
    }

}

YACK_UTEST(memory_operative)
{

    const size_t n1     = 100;
    const size_t bytes1 = n1 * sizeof(apq);
    void *impl1[YACK_WORDS_GEQ(bytes1)];

    const size_t n2     = 17;
    const size_t bytes2 = n2 * sizeof(apq);
    void *impl2[YACK_WORDS_GEQ(bytes2)];


    memory::operative op1(impl1,n1,sizeof(apq),start,clear);
    memory::operative op2(impl2,n2,sizeof(apq),start,clear);

    std::cerr << "Created!" << std::endl;

    op1.swap_with(op2);


}
YACK_UDONE()

