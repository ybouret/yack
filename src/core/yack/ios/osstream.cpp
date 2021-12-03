#include "yack/ios/osstream.hpp"


namespace yack
{
    namespace ios
    {

        osstream:: ~osstream() throw() {}

        osstream:: osstream(string &target) throw() : host(target) {}

        void osstream:: write(char C)
        {
            host += C;
        }

        void osstream:: flush()
        {

        }

    }

}
