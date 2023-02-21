
#include "yack/jive/lexical/plugin/multi-lines-comment.hpp"

namespace yack
{
    namespace jive
    {

        namespace lexical
        {
            multi_lines_comment:: ~multi_lines_comment() noexcept
            {
            }

            void multi_lines_comment:: compile()
            {
                endl("[:endl:]");
                drop(".");
            }

            void multi_lines_comment:: enter(token &) noexcept
            {

            }

            void multi_lines_comment:: leave(token &) noexcept
            {

            }

            YACK_JIVE_PLUGIN_IMPL(c_comments);

        }
    }
}

