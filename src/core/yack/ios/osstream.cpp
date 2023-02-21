#include "yack/ios/osstream.hpp"


namespace yack
{
    namespace ios
    {

        osstream:: ~osstream() noexcept {}

        osstream:: osstream(string &target) noexcept : host(target) {}

        void osstream:: write(char C)
        {
            host += C;
        }

        void osstream:: flush()
        {

        }

    }

}
