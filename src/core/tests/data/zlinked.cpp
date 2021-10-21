#include "yack/data/zlist.hpp"
#include "yack/data/zpool.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace {

    class dummy
    {
    public:
        dummy *next;
        dummy *prev;

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(dummy);
    };

}

YACK_UTEST(data_zlinked)
{

    dummy *d = object::zacquire<dummy>();
    object::zrelease(d);

    zlist_of<dummy> dzl;


    zpool_of<dummy> dzp;
    list_of<dummy>  dl;



}
YACK_UDONE()


