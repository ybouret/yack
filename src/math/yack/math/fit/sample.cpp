
#include "yack/math/fit/sample.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace math
    {

        namespace fit
        {
            sample_:: ~sample_() throw()
            {
            }

            const string & sample_:: key() const throw() { return name; }
            
            void sample_:: throw_multiple_name(const string &id) const
            {
                throw exception("fit::samples(multile '%s')", id());
            }

            const variables & sample_:: bulk() const throw() { return vars; }
        }

    }

}

