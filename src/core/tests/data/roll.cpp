#include "yack/sequence/roll.hpp"
#include "yack/utest/run.hpp"

#include "yack/string.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/sequence/thin-array.hpp"

using namespace yack;

YACK_UTEST(data_roll)
{
    int data[] = { 1, 2, 3, 4};

    thin_array<int> iv(data,sizeof(data)/sizeof(data[0]));
    std::cerr << iv << std::endl;
    for(size_t i=iv.size();i>0;--i)
    {
        rolling::down(iv);
        std::cerr << iv << std::endl;
    }

    string s = "Hello, World!";
    std::cerr << s << std::endl;
    for(size_t i=s.size();i>0;--i)
    {
        rolling::down(s);
        std::cerr << s << std::endl;
    }

}
YACK_UDONE()

