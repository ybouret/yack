#include "yack/jive/module.hpp"
#include "yack/utest/run.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/string.hpp"

using namespace yack;


YACK_UTEST(token)
{
    //concurrent::single::verbose = true;
    
    if(argc>1)
    {
        const string           filename = argv[1];
        auto_ptr<jive::module> m =  (filename != YACK_STDIN) ? jive::module::open_file(argv[1]) : jive::module::open_stdin();
        std::cerr << "opened " << m << std::endl;
        
        jive::token tkn;
        while( m->gets(tkn) )
        {
            if(tkn.size)
            {
                std::cerr << tkn;
                const jive::context &ctx = *tkn.head;
                std::cerr << "@" << ctx << ":" << ctx.line << ":" << ctx.column << "+" << tkn.size;
                std::cerr << std::endl;
            }
                                     
        }
        
    }
}
YACK_UDONE()

