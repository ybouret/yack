#include "yack/string/string.hpp"
#include "yack/string/utf8.hpp"
#include "yack/string.hpp"
#include "yack/utest/run.hpp"

using namespace yack;

YACK_UTEST(strings)
{
    {
        kernel::string<char> s1; std::cerr << "s1.width=" << s1.width << std::endl;
        kernel::string<utf8> s2; std::cerr << "s2.width=" << s2.width << std::endl;
    }

    {
        string s; std::cerr << s << std::endl;
    }

    {
        string s = 'a'; std::cerr << s << std::endl;
    }

    {
        string s = ""; std::cerr << s << std::endl;
    }

    {
        string s = "a"; std::cerr << s << std::endl;
    }

    {
        string s = "ab"; std::cerr << s << std::endl;
    }

    {
        string lhs = " hello ";
        string rhs = " world ";
        {
            string s = lhs + rhs; std::cerr << s << std::endl;
        }

        {
            string s = lhs + 'a'; std::cerr << s << std::endl;
        }

        {
            string s = 'a' + rhs; std::cerr << s << std::endl;
        }

        {
            string s = lhs + "you"; std::cerr << s << std::endl;
        }

        {
            string s = "damn" + rhs; std::cerr << s << std::endl;
            std::cerr << s.skip(1) << std::endl;
            std::cerr << s.skip(2) << std::endl;

            std::cerr << s.trim(1) << std::endl;
            std::cerr << s.trim(2) << std::endl;

        }

    }

}
YACK_UDONE()

