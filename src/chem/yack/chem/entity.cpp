
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
        ios::vizible(),
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


        ios::ostream & entity:: viz(ios::ostream &os) const
        {
            logo(os) << '[';
            add_label(os,name());
            os << attr();
            os << ']';
            end(os);
            return os;
        }

    }

}

