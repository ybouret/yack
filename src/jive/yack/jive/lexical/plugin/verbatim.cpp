
#include "yack/jive/lexical/plugin/verbatim.hpp"

namespace yack
{
    namespace jive
    {

        namespace lexical
        {
            YACK_JIVE_PLUGIN_IMPL(verbatim);

            verbatim:: ~verbatim() throw() {}

            behavior verbatim:: on_dot(token &t) throw()
            {
                content.merge_back(t);
                return discard;
            }

            behavior verbatim:: on_endl(token &t) throw()
            {
                assert(flux);
                content.merge_back(t);
                (**flux).newline();
                return discard;
            }

            void verbatim:: setup()
            {
                make("[:dot:]", this, &verbatim::on_dot);
                make("[:endl:]",this, &verbatim::on_endl);
            }

            void verbatim:: enter(token &t) throw()
            {
                assert(t.size>0);
                content.swap_with(t);
                coerce(to_skip) = content.size;
            }

            void verbatim:: leave(token &)
            {
                assert(content.size>=to_skip);
                token &result = *inject(content);
                assert(result.size>=to_skip);
                result.skip(to_skip);
                coerce(to_skip) = 0;
            }

        }

    }
}


