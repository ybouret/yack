
#include "yack/jive/lexical/plugin/multi-lines-comment.hpp"

namespace yack
{
    namespace jive
    {

        namespace lexical
        {
            multi_lines_comment:: ~multi_lines_comment() throw()
            {
            }

            void multi_lines_comment:: compile()
            {
                endl("[:endl:]");
                drop(".");
            }

            void multi_lines_comment:: enter(token &) throw()
            {

            }

            void multi_lines_comment:: leave(token &) throw()
            {

            }

            YACK_JIVE_PLUGIN_IMPL(c_comments);

        }
    }
}

