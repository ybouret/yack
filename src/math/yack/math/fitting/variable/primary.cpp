#include "yack/math/fitting/variable/primary.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {
            primary:: ~primary() throw()
            {
                coerce(indx_) = 0;
            }

            primary:: primary(const string &id, const size_t i) :
            variable(id),
            indx_(i)
            {
                assert(i>0);
            }

            size_t primary:: indx() const throw() {
                assert(indx_>0);
                return indx_;
            }

            const string & primary:: alias() const throw()
            {
                return name;
            }

            bool primary:: is_primary() const throw() { return true; }
        }

    }
}