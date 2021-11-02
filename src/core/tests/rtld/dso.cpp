#include "yack/rtld/dso.hpp"
#include "yack/utest/run.hpp"
#include "yack/ios/fmt/hexa.hpp"

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

        cfunc Sin = dll.proc<cfunc>("Sin");
        if(Sin)
        {
            std::cerr << "Found Sin" << std::endl;
            for(double x=0; x <= 1.0; x+=0.1)
            {
                std::cerr << "Sin(" << x << ")=" << Sin(x) << std::endl;
            }
        }

        void *p = dll.load("Data");
        if( p )
        {
            std::cerr << "Found Data @" <<  p << std::endl;
            const uint32_t value = *(uint32_t *)p;
            std::cerr << ios::hexa(value) << std::endl;
        }


    }
}
YACK_UDONE()

