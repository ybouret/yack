#include "yack/math/fitting/variable/replica.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {
            replica:: ~replica() throw()
            {
            }

            replica:: replica(const string &id, const primary::handle &hp) :
            variable(id),
            link(hp)
            {

            }

            const string & replica:: alias() const throw()
            {
                return link->name;
            }

            size_t replica:: indx() const throw()
            {
                return **link;
            }

            bool replica:: is_primary() const throw() { return false; }


        }

    }

}

