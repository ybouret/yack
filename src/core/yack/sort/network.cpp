

#include "yack/sort/network.hpp"

namespace yack
{

    namespace nwsrt
    {
        algorithm:: algorithm(const swaps &instr) noexcept :
        code(instr),
        buff(code.name)
        {
        }

        algorithm:: ~algorithm() noexcept
        {
        }
        

        const memory::ro_buffer & algorithm:: key() const noexcept { return buff; }

        

    }

}

