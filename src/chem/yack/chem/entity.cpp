
#include "yack/chem/entity.hpp"
#include "yack/ios/ostream.hpp"

namespace yack
{
    namespace chemical
    {
        bool entity::verbose = false;

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

