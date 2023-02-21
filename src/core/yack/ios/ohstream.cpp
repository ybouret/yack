
#include "yack/ios/ohstream.hpp"


namespace yack
{
    namespace ios
    {

        ohstream:: ~ohstream() noexcept {}

        ohstream:: ohstream(hashing::function &h) noexcept : host(h) {}

        void ohstream:: write(char C)
        {
            host.run(&C,1);
        }

        void ohstream:: flush()
        {

        }

    }

}
