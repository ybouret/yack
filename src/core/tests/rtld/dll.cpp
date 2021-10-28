#include "yack/rtld/dll.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(rtld_dll)
{
    if(argc>1)
    {
        const char *soname = argv[1];
        dll DLL(soname);
        std::cerr << "Loaded " << soname << std::endl;

        for(int i=2;i<argc;++i)
        {
            const char *symbol = argv[i];
            void *addr = DLL.load(symbol);
            std::cerr << std::setw(32) << symbol << " @" << addr << std::endl;
        }

    }
}
YACK_UDONE()

