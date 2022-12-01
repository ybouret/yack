
#include "yack/chem/conservation-law.hpp"

namespace yack
{
    namespace chemical
    {

        conservation_law:: ~conservation_law() throw() {}
        conservation_law::  conservation_law() throw() : object(), actors(), next(0), prev(0) {}


        std::ostream & operator<<(std::ostream &os, const conservation_law &self)
        {
            const actors &me = self;
            os << "0=d(" << me << ")";
            return os;
        }

    }

}

namespace yack
{
    namespace chemical
    {

        claw_team :: claw_team() throw()
        {

        }

        claw_team:: ~claw_team() throw()
        {
            
        }

    }
}
