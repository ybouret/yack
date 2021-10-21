#include "yack/data/zlist.hpp"
#include "yack/data/zpool.hpp"
#include "yack/data/xlist.hpp"
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
    randomized::rand_ ran;

    {
        zlist_of<dummy> dzl; dzl.reserve(20);
        zpool_of<dummy> dzp; dzp.reserve(20);

        {
            list_of<dummy>  dl;
            for(size_t i=1;i<=25;++i)
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

        YACK_SIZEOF(zlist_of<dummy>);
        YACK_SIZEOF(zpool_of<dummy>);
        YACK_OUTPUT(dzl.size());
        YACK_OUTPUT(dzp.size());
    }

    xlist<int>                        ixl;
    zpool_of< xlist<int>::node_type > izp;

    izp.reserve(5);
    for(size_t i=0;i<=12;++i)
    {
        ixl.push_back(int(i),izp);
    }
    randomized::shuffle::list( *ixl, ran );
    for(xlist<int>::node_type *node = (*ixl).head; node; node=node->next)
    {
        std::cerr << "/" << **node;
    }
    std::cerr << std::endl;


    while( (*ixl).size )
    {
        ixl.pop_back(izp);
    }



    

}
YACK_UDONE()


