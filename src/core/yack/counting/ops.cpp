#include "yack/counting/ops.hpp"

namespace yack
{

    namespace nubbin
    {
        void counting:: initialize(writable<size_t> &state) throw()
        {
            for(size_t i=state.size();i>0;--i)
            {
                state[i] = i;
            }
        }
    }

}

