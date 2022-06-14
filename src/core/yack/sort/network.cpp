

#include "yack/sort/network.hpp"

namespace yack
{

    namespace nwsrt
    {
        algorithm:: algorithm(const swaps &instr) throw() :
        code(instr),
        buff(code.name)
        {
        }

        algorithm:: ~algorithm() throw()
        {
        }
        

        const memory::ro_buffer & algorithm:: key() const throw() { return buff; }

        

    }

}

