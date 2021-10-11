

#include "yack/object.hpp"
#include "yack/utest/run.hpp"
#include "yack/data/cxx-list.hpp"
#include "yack/arith/align.hpp"
#include <cstring>
#include <cerrno>

using namespace yack;

namespace {

    static size_t all_blocks = 0;

    template <size_t N>
    class block : public object
    {
    public:
        block *next;
        block *prev;
        void  *data[N];

        block() throw() : next(0), prev(0), data()
        {
            memset(data,0,sizeof(data));
            ++all_blocks;
        }

        ~block() throw()
        {
            --all_blocks;
        }



    private:
        YACK_DISABLE_COPY_AND_ASSIGN(block);
    };




    template <size_t N> static inline
    void do_test_obj(uprng &ran)
    {
        typedef block<N> Block;
        std::cerr << "N=" << N << ": ";YACK_SIZEOF(block<N>);
        {
            cxx_list_of<Block> l;
            for(size_t i=1000+ran.leq(1000);i>0;--i)
            {
                l.push_back(new Block());
            }

            ran.shuffle_list(l);
        }
        YACK_CHECK("new/delete" && all_blocks<=0);
        for(size_t i=0;i<=1000;++i)
        {
            Block *B = new Block[i];
            delete []B;
        }
        YACK_CHECK("new[]/delete[]" && all_blocks<=0);

        void *impl[ YACK_WORDS_FOR(Block) ];
        ran.fillnz(impl,sizeof(impl));
        Block *b = new (impl) Block();
        b->~Block();
        YACK_CHECK("placement" && all_blocks<=0);


    }


}

YACK_UTEST(object)
{
    uprng ran;
    do_test_obj<0>(ran);
    do_test_obj<1>(ran);
    do_test_obj<2>(ran);
    do_test_obj<3>(ran);
    do_test_obj<4>(ran);
    do_test_obj<5>(ran);
    do_test_obj<6>(ran);
    do_test_obj<7>(ran);

}
YACK_UDONE()

