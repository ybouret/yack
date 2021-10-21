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
        const size_t indx;

        dummy(const size_t i) throw() :
        next(0), prev(0), indx(i) {}

        ~dummy() throw()
        {
        }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(dummy);
    };

}

YACK_UTEST(data_zlinked)
{


    zlist_of<dummy> dzl; dzl.reserve(20);
    zpool_of<dummy> dzp; dzp.reserve(20);

    list_of<dummy>  dl;
    for(size_t i=1;i<=100;++i)
    {
        dl.push_back(  dzl.zquery() );
        dl.push_front( dzp.zquery()  );
    }

    while(dl.size)
    {
        dzl.zstore( dl.pop_front() );
        dzp.zstore( dl.pop_back()  );
    }


}
YACK_UDONE()


