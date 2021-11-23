#include "yack/string/string.hpp"
#include "yack/string/utf8.hpp"
#include "yack/string.hpp"
#include "yack/string/ops.hpp"
#include "yack/utest/run.hpp"
#include "../main.hpp"
#include "yack/sequence/vector.hpp"

using namespace yack;


static inline bool is_space(const int ch) throw()
{
    return ' ' == ch;
}

YACK_UTEST(strings)
{
    randomized::rand_ ran;

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

        {
            string ans = vformat("  %s  ", "1234");
            std::cerr << "ans = '" <<  ans << "'" << std::endl;
            std::cerr << "      '" << strops::clean_if(is_space,ans) << "'" << std::endl;
        }
    }

    {
        vector<string> strings;
        for(size_t i=3+ran.leq(10);i>0;--i)
        {
            const string tmp = bring::get<string>(ran);
            strings.push_back(tmp);
            strings.push_front(tmp);
        }
        std::cerr << strings << std::endl;
        strings.sort( string::compare );
        std::cerr << strings << std::endl;
    }


}
YACK_UDONE()

