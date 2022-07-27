#include "yack/ios/pstream.hpp"
#include "yack/utest/run.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/string.hpp"

using namespace yack;

YACK_UTEST(ios_pstream)
{
    ios::pstream::result retval = new int;
    if(argc>1)
    {
		std::cerr << "\t<Starting '" << argv[1] << "'>" << std::endl;
        vector<string> lines;
		{
			ios::pstream fp(argv[1], retval);
            fp.load(lines);
        }
        std::cerr << lines << std::endl;
        std::cerr << "\t<Result '" << argv[1] << "' = " << retval << ">" << std::endl;
    }
}
YACK_UDONE()
