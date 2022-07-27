#include "yack/ios/pstream.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(ios_pstream)
{
    ios::pstream::result retval = new int;
    if(argc>1)
    {
		std::cerr << "\t<Starting '" << argv[1] << "'>" << std::endl;
		{
			ios::pstream fp(argv[1], retval);
			ios::characters line;
			while (fp.gets(line))
			{
				std::cerr << line << std::endl;
			}
        }
        std::cerr << "\t<Result '" << argv[1] << "' = " << retval << ">" << std::endl;
        
    }
}
YACK_UDONE()
