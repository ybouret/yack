
#include "yack/chem/eqs/squad.hpp"

namespace yack
{
    namespace chemical
    {

        eq_squad::  eq_squad() throw() :  eq_team(), next(0), prev(0) {}
        eq_squad:: ~eq_squad() throw() {}

    }

}


namespace yack
{
    namespace chemical
    {

        eq_squads::  eq_squads() throw() :  cxx_list_of<eq_squad>() {}
        eq_squads:: ~eq_squads() throw() {}

    }

}

