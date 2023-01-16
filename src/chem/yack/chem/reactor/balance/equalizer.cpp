

#include "yack/chem/reactor/balance/equalizer.hpp"

namespace yack {

    namespace chemical
    {

        equalizer:: ~equalizer() throw() {}


        equalizer:: equalizer() :
        sp_fund( new sp_bank() ),
        fence( *this )
        {
        }

    }

}
