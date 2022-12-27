
#include "yack/chem/equalizer.hpp"

namespace yack
{

    namespace chemical
    {

        equalizer:: ~equalizer() throw()
        {
        }

        equalizer:: equalizer(const size_t m, const sp_fund &io) :
        pos(io),
        neg(m,io)
        {
        }

        void equalizer:: reset() throw()
        {
            pos.destroy();
            neg.destroy();
        }


    }

}

