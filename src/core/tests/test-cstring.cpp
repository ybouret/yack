
#include "yack/type/cstring.h"
#include "yack/utest/run.hpp"

YACK_UTEST(cstring)
{
    char         buffer[16];
    const size_t buflen = sizeof(buffer);

    yack_cstring_chrcpy(buffer,buflen,'a');
    std::cerr << buffer << std::endl;
    yack_cstring_chrcat(buffer,buflen,'b');
    std::cerr << buffer << std::endl;
    yack_cstring_msgcpy(buffer,buflen,"ba");
    std::cerr << buffer << std::endl;
    yack_cstring_msgcat(buffer,buflen,"ab");
    std::cerr << buffer << std::endl;

    for(int i=1;i<argc;++i)
    {
        yack_cstring_msgcat(buffer,buflen,argv[i]);
        std::cerr << buffer << std::endl;
    }
    



}
YACK_UDONE()

