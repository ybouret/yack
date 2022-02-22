
#include "yack/chem/interactions.hpp"
#include "yack/exception.hpp"
#include "yack/type/utils.hpp"



namespace yack
{
    namespace chemical
    {
        connexions:: ~connexions() throw()
        {
        }

        connexions:: connexions(const equilibrium &eq) throw() :
        authority<const equilibrium>(eq)
        {
            
        }
    }

}

