#include "yack/data/slim/list.hpp"
#include "yack/data/slim/para-list.hpp"
#include "yack/data/slim/pool.hpp"
#include "yack/data/slim/ptr.hpp"
#include "yack/utest/run.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/string.hpp"

using namespace  yack;

YACK_UTEST(data_slim)
{


    YACK_SIZEOF(slim_node<string>);
    YACK_SIZEOF(slim_node<int>);
    std::cerr << std::endl;


    YACK_SIZEOF(slim_node< slim_ptr<string> > );
    YACK_SIZEOF(slim_node< slim_ptr<int> >    );
    std::cerr << std::endl;

    YACK_SIZEOF(slim_list<int>);
    YACK_SIZEOF(slim_list<string>);
    std::cerr << std::endl;

    YACK_SIZEOF(slim_pool<int>);
    YACK_SIZEOF(slim_pool<string>);
    std::cerr << std::endl;

    const string world = "world";

    {
        auto_ptr< slim_node<string> > ps1 = new slim_node<string>();
        auto_ptr< slim_node<string> > ps2 = new slim_node<string>("hello",transmogrify);
        auto_ptr< slim_node<string> > ps3 = new slim_node<string>(world,transmogrify);
        auto_ptr< slim_node<string> > ps4 = new slim_node<string>('A',transmogrify);

        std::cerr << ps1 << std::endl;
        std::cerr << ps2 << std::endl;
        std::cerr << ps3 << std::endl;
        std::cerr << ps4 << std::endl;
    }

    {
        size_t arr[] = {0,1,2,3};
        slim_ptr<size_t> sp1;
        slim_ptr<size_t> sp2 = &arr[2];
        slim_ptr<size_t> sp3 = sp2;

        std::cerr << sp1 << std::endl;
        std::cerr << sp2 << std::endl;
        std::cerr << sp3 << std::endl;

        slim_ptr<const size_t> csp1;
        std::cerr << csp1 << std::endl;
        slim_ptr<const size_t> csp0 = arr;
        std::cerr << csp0 << std::endl;

        slim_ptr<const size_t> csp3 = (const size_t *) &arr[3];
        std::cerr << csp3 << std::endl;
        std::cerr << *csp3 << std::endl;
    }

    {
        char chr[] = { 'y', 'e', 's', 0 };
        slim_node< slim_ptr<char> > pc;
        std::cerr << pc << std::endl;

        slim_node< slim_ptr<char> > pc0( &chr[0],transmogrify);
        std::cerr << pc0 << std::endl;
    }


    {
        slim_list<string> sl;

        sl << world;
        sl << '!';
        sl >> "hello";

        std::cerr << sl << std::endl;

        for(size_t i=sl.size;i>0;--i)
        {
            std::cerr << "\t" << sl[i] << std::endl;
        }

        para_list<string>       mpl;
        para_list<const string> cpl;


        slim_pool<string> sp;

        sp.reserve(10);

        while(sl.size) {
            sp.zstore( sl.pop_front() );
        }

    }





}
YACK_UDONE()
