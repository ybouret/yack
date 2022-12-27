
#include "yack/chem/balancing/equalizer.hpp"

namespace yack
{

    namespace chemical
    {

        equalizer:: ~equalizer() throw()
        {
        }

        equalizer:: equalizer(const size_t m, const sp_fund &io) :
        lim(io),
        neg(m,io)
        {
        }

        void equalizer:: destroy() throw()
        {
            lim.destroy();
            neg.destroy();
        }

        void equalizer:: look_up(frontier &fade, const equalizer &other) const
        {
            neg.analyze(fade,other.lim);
        }


    }

}

