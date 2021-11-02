
#include "yack/info/modulation/delta.hpp"
#include <cstring>

namespace yack
{

    namespace information
    {
        delta:: ~delta() throw()
        {
            last = 0;
        }


        delta:: delta() throw() : last(0)
        {
        }

        void delta:: reset() throw()
        {
            last = 0;
        }

    }

}


namespace yack
{

    namespace information
    {

        delta_encoder:: delta_encoder() throw() : delta()
        {

        }

        delta_encoder:: ~delta_encoder() throw()
        {
        }

        uint8_t delta_encoder:: alter(const uint8_t x) throw()
        {
            const uint8_t ans = x-last;
            last = x;
            return ans;
        }
    }

}

namespace yack
{

    namespace information
    {

        delta_decoder:: delta_decoder() throw() : delta()
        {

        }

        delta_decoder:: ~delta_decoder() throw()
        {
        }

        uint8_t delta_decoder:: alter(const uint8_t x) throw()
        {
            const uint8_t ans = x+last;
            last = ans;
            return ans;
        }

    }

}
