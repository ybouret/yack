#include "yack/chem/entity.hpp"

namespace yack
{
    namespace chemical
    {

        entity:: ~entity() throw()
        {
        }

        bool entity:: index_in(const collection &arr) const throw()
        {
            return indx>=1 && indx <= arr.size();
        }

        const string & entity:: key() const throw()
        {
            return name;
        }

        size_t entity:: operator*() const throw()
        {
            return indx;
        }



    }

}

