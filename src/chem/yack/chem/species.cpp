#include "yack/chem/species.hpp"

namespace yack
{

    namespace chemical
    {

        species:: ~species() throw() {}

        const string & species:: key() const throw()
        {
            return name;
        }

        std::ostream & operator<<(std::ostream &os, const species &s)
        {
            std::cerr << s.name;
            return os;
        }

    }
}

