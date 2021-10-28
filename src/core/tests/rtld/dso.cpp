#include "yack/rtld/dso.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace
{
    typedef double  (*cfunc)(double);
}

YACK_UTEST(rtld_dso)
{
    if(argc>1)
    {
        const char *soname = argv[1];
        dso dll(soname);
        std::cerr << "Loaded " << soname << std::endl;

        for(int i=2;i<argc;++i)
        {
            const char *symbol = argv[i];
            void *addr = dll.load(symbol);
            std::cerr << std::setw(32) << symbol << " @" << addr << std::endl;
        }

        cfunc Sin = dll.query<cfunc>("Sin");
        if(Sin)
        {
            std::cerr << "Found Sin" << std::endl;
            for(double x=0; x <= 1.0; x+=0.1)
            {
                std::cerr << "Sin(" << x << ")=" << Sin(x) << std::endl;
            }
        }


    }
}
YACK_UDONE()

