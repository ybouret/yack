
#include "yack/ios/ohstream.hpp"


namespace yack
{
    namespace ios
    {

        ohstream:: ~ohstream() throw() {}

        ohstream:: ohstream(hashing::function &h) throw() : host(h) {}

        void ohstream:: write(char C)
        {
            host.run(&C,1);
        }

        void ohstream:: flush()
        {

        }

    }

}
