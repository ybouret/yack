
#include "yack/jive/lexical/analyzer.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace jive
    {

        namespace lexical
        {
            plugin:: ~plugin() throw()
            {}


            void plugin:: on_call(token &word)
            {
                enter(word);
            }

            void plugin:: inject(token &word) const
            {
                if(word.size<=0) throw exception("<%s> returned empty", (*label)());
                lexeme *lx = new lexeme(label,*word);
                (**lx).swap_with(word);
                root->store(lx);
            }


        }

    }
}
