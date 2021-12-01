
#include "yack/jive/module.hpp"
#include "yack/utest/run.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/string.hpp"

using namespace yack;

YACK_UTEST(source)
{
    if(argc>1)
    {
        const string filename = argv[1];
        auto_ptr<jive::module> m =  (filename != YACK_STDIN) ? jive::module::open_file(argv[1]) : jive::module::open_stdin();
        std::cerr << "opened " << m << std::endl;
        jive::character *ch; //= new jive::character(*m,'a');
        while( NULL != (ch = m->get()) )
        {
            const auto_ptr<jive::character> p = ch;
            std::cerr << **p;
        }
        
    }
}
YACK_UDONE()

