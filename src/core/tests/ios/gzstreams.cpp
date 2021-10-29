
#include "yack/ios/z/igzstream.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

namespace
{

}

YACK_UTEST(ios_gzstreams)
{
    if(argc>1)
    {
        {
            std::cerr << "// byte wise..." << std::endl;
            ios::igzstream fp(argv[1]);
            char C;
            while( fp.query(C) )
            {
                std::cerr << C;
            }
        }
        
        {
            std::cerr << "// block wise..." << std::endl;
            ios::igzstream fp(argv[1]);
            const size_t block_size = 256;
            char         block_addr[block_size];
            size_t       nr = 0;
            while( (nr=fp.fetch(block_addr,block_size)) >0 )
            {
                for(size_t i=0;i<nr;++i)
                    std::cerr << block_addr[i];
            }
            
            
            
        }
        
    }
}
YACK_UDONE()

