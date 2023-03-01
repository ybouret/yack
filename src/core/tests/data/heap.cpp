#include "yack/ordered/prio-queue.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"

using namespace yack;

namespace
{
    struct icompare
    {
        inline int operator()(const int lhs, const int rhs) noexcept {
            return lhs-rhs;
        }
    };

    template <typename T, const size_t N>
    class fixed_buffer
    {
    public:
        static const size_t type_size = sizeof(T);
        static const size_t wksp_size = type_size * N;

        inline fixed_buffer() noexcept :
        workspace(0),
        wksp() {
            YACK_STATIC_ZSET(wksp);
            workspace = coerce_cast<T>(&wksp[0]);
        }

        inline ~fixed_buffer() noexcept
        {
            YACK_STATIC_ZSET(wksp);
        }

        T                  *workspace;
        static const size_t num_items = N;


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(fixed_buffer);
        void *wksp[ YACK_WORDS_GEQ(wksp_size) ];
    };
}

YACK_UTEST(data_heap)
{
    randomized::rand_ ran;


    fixed_buffer<int,4> data;

    prio_queue<int,icompare> Q(data.workspace,data.num_items);

    Q.insert(-1);
    Q.insert(2);
    Q.insert(-3);
    Q.insert(4);


}
YACK_UDONE()
