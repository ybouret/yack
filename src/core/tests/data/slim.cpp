#include "yack/data/slim/node.hpp"
#include "yack/utest/run.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/string.hpp"

using namespace  yack;

YACK_UTEST(data_slim)
{

    YACK_SIZEOF(slim_node<string>);
    YACK_SIZEOF(slim_node<int>);

    auto_ptr< slim_node<string> > ps1 = new slim_node<string>();
    auto_ptr< slim_node<string> > ps2 = new slim_node<string>("hello",transmogrify);
    const string world = "world";
    auto_ptr< slim_node<string> > ps3 = new slim_node<string>(world,transmogrify);
    auto_ptr< slim_node<string> > ps4 = new slim_node<string>('y',transmogrify);

    std::cerr << ps1 << std::endl;
    std::cerr << ps2 << std::endl;
    std::cerr << ps3 << std::endl;
    std::cerr << ps4 << std::endl;


}
YACK_UDONE()
