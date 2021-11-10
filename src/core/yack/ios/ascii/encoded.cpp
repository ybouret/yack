
#include "yack/ios/ascii/encoded.hpp"

namespace yack
{

    namespace ios
    {

        namespace ascii
        {

            const char encoded[256][4] =
            {
#include       "encoded.hxx"
            };
        }

    }

}

