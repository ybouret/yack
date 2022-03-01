#include "yack/chem/entity.hpp"

namespace yack
{
    namespace chemical
    {
        entity:: ~entity() throw() {}

        size_t entity:: operator*() const throw()
        {
            return indx;
        }

        const string & entity:: key() const throw()
        {
            return name;
        }


    }
}
