


#include "yack/ios/icstream.hpp"

namespace yack
{
    namespace ios
    {

        icstream:: ~icstream() throw()
        {
        }


        icstream:: icstream(const cstdin_t &_) :
        file(_)
        {
        }

    }

}
