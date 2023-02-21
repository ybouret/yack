#include "yack/math/fitting/variable/replica.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {
            replica:: ~replica() noexcept
            {
            }

            replica:: replica(const string &id, const primary::handle &hp) :
            variable(id),
            link(hp)
            {

            }

            const string & replica:: alias() const noexcept
            {
                return link->name;
            }

            size_t replica:: indx() const noexcept
            {
                return **link;
            }

            bool replica:: is_primary() const noexcept { return false; }


        }

    }

}

