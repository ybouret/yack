#include "yack/ios/ascii/convert.hpp"
#include "yack/utest/run.hpp"
#include "yack/string.hpp"

using namespace yack;

YACK_UTEST(ios_convert)
{
    std::cerr << integral_for<int64_t>::minimum << std::endl;
    std::cerr << integral_for<int64_t>::maximum << std::endl;

    std::cerr << integral_for<uint64_t>::maximum << std::endl;

    for(int i=1;i<argc;++i)
    {
        string args = argv[i];
        if(args.size()>0)
        {
            switch(args[1])
            {
                case 'i':
                {
                    args.skip(1);
                    int64_t I = ios::ascii::convert::to_int64(args,"int64_t");
                    std::cerr << "I=" << I << std::endl;
                } break;

                case 'u':
                {
                    args.skip(1);
                    uint64_t U = ios::ascii::convert::to_uint64(args,"uint64_t");
                    std::cerr << "U=" << U << std::endl;
                } break;

                default:
                    throw exception("unknown prefix '%c'",args[1]);
            }
        }
    }

}
YACK_UDONE()

