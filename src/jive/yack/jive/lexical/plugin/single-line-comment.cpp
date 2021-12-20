#include "yack/jive/lexical/plugin/single-line-comment.hpp"

namespace yack
{
    namespace jive
    {

        namespace lexical
        {
            single_line_comment:: ~single_line_comment() throw()
            {
            }

            void single_line_comment:: compile()
            {
                back("[:endl:]",this,&single_line_comment::leave);
                drop(".+");
            }

            void single_line_comment:: enter(token &) throw()
            {

            }

            void single_line_comment:: leave(token &word) throw()
            {
                // word is endl => restore for host
                restore(word);
            }

            YACK_JIVE_PLUGIN_IMPL(cxx_comment);
        }
    }
}

