
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
        indexed(other),
        name(other.name)
        {
        }
        
        

        const string & entity:: key() const throw()
        {
            return name;
        }


        
    }

}

