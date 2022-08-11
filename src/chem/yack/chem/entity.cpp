
#include "yack/chem/entity.hpp"

namespace yack
{
    namespace chemical
    {

        entity:: ~entity() throw()
        {
        }

        entity:: entity(const entity &other) :
        large_object(),
        counted(),
        name(other.name),
        indx(other.indx)
        {
        }
        
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

