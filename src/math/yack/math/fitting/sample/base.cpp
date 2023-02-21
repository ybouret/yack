
#include "yack/math/fitting/sample/base.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {

            const char sample_::clid[] = "fitting::sample";

            sample_:: ~sample_() noexcept
            {
            }

            const string & sample_:: key() const noexcept { return name; }

            void sample_:: throw_multiple_name(const string &id) const
            {
                throw imported::exception(clid,"multiple '%s'", id());
            }

            void sample_:: throw_unknown_name(const string &id) const
            {
                throw imported::exception(clid,"no '%s'", id());
            }

            variables    & sample_:: operator*() noexcept
            {
                return vars_;
            }


            const variables & sample_:: operator*() const noexcept
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

