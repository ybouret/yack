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
                    int64_t I = ios::ascii::convert::to<int64_t>(args,"int64_t");
                    std::cerr << "I=" << I << std::endl;
                } break;

                case 'u':
                {
                    args.skip(1);
                    uint64_t U = ios::ascii::convert::to<uint64_t>(args,"uint64_t");
                    std::cerr << "U=" << U << std::endl;
                } break;

                case 'c':
                {
                    args.skip(1);
                    int8_t c = ios::ascii::convert::to<int8_t>(args,"int8_t");
                    std::cerr << "c=" << int64_t(c) << std::endl;
                } break;

                case 'b':
                {
                    args.skip(1);
                    uint8_t c = ios::ascii::convert::to<uint8_t>(args,"uint8_t");
                    std::cerr << "b=" << uint64_t(c) << std::endl;
                } break;

                case 's':
                {
                    args.skip(1);
                    int16_t s = ios::ascii::convert::to<int16_t>(args,"int16_t");
                    std::cerr << "s=" << int64_t(s) << std::endl;
                } break;

                case 'w':
                {
                    args.skip(1);
                    uint16_t w = ios::ascii::convert::to<uint16_t>(args,"uint16_t");
                    std::cerr << "w=" << uint64_t(w) << std::endl;
                } break;

                case 'f':
                {
                    args.skip(1);
                    float f = ios::ascii::convert::real<float>(args,"float");
                    std::cerr << "f=" << f << std::endl;
                } break;

                case 'd':
                {
                    args.skip(1);
                    double d = ios::ascii::convert::real<double>(args,"double");
                    std::cerr << "d=" << d << std::endl;
                } break;

                case 'e':
                {
                    args.skip(1);
                    double e = ios::ascii::convert::real<long double>(args,"long double");
                    std::cerr << "e=" << e << std::endl;
                } break;

                default:
                    throw exception("unknown prefix '%c'",args[1]);
            }
        }
    }

}
YACK_UDONE()

