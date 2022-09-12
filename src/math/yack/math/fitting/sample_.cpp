
#include "yack/math/fitting/sample_.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {

            const char sample_::clid[] = "fitting::sample";

            sample_:: ~sample_() throw()
            {
            }

            const string & sample_:: key() const throw() { return name; }

            void sample_:: throw_multiple_name(const string &id) const
            {
                throw imported::exception(clid,"multiple '%s'", id());
            }

            void sample_:: throw_unknown_name(const string &id) const
            {
                throw imported::exception(clid,"no '%s'", id());
            }
        }

    }

}

