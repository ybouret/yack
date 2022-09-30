
#include "yack/math/fitting/sample/base.hpp"
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

            variables    & sample_:: operator*() throw()
            {
                return vars_;
            }


            const variables & sample_:: operator*() const throw()
            {
                return vars_;
            }

            namespace
            {
                static const char csid[] = "fitting::csample";
            }
            
            void sample_:: throw_abscissa_failure()
            {
                throw imported::exception(csid,"invalid abscissa(e) copy");

            }

            void sample_:: throw_ordinate_failure()
            {
                throw imported::exception(csid,"invalid ordinate(s) copy");
            }

        }

    }

}

