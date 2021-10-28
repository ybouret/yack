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
    }
}
YACK_UDONE()

