
#include "yack/hashing/testing.hpp"
#include "yack/hashing/function.hpp"
#include "yack/kr/digest.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace hashing
    {

        void testing:: run( function &H, const testing tests[], const size_t count )
        {
            assert(tests);
            std::cerr << "<hashing::test " << H.name() << ">" <<std::endl;
            for(size_t i=0;i<count;++i)
            {
                const testing &the_test = tests[i];
                assert(the_test.text);
                assert(the_test.hash);
                const digest hash = the_test.hash;
                const digest hrun = H.md(the_test.text);
                std::cerr << "  " << hrun << '/' << hash << " <== '" << the_test.text << "'" << std::endl;
                if(hrun!=hash) throw exception("[%s] test failure", H.name() );
            }
            std::cerr << "<hashing::test " << H.name() << "/>" <<std::endl;

        }

    }
}
