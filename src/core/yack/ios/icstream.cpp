


#include "yack/ios/icstream.hpp"

namespace yack
{
    namespace ios
    {

        icstream:: ~icstream() noexcept
        {
        }


        icstream:: icstream(const cstdin_t &_) :
        file(_)
        {
        }


        icstream:: icstream(const char *filename) :
        file(filename)
        {
        }

        icstream:: icstream(const string &filename) :
        file(filename)
        {
        }

        bool icstream:: query_(char &C)
        {
            return file.get(C);
        }

        size_t icstream:: fetch_(void *addr, const size_t size)
        {
            return file.get(addr,size);
        }

    }

}
