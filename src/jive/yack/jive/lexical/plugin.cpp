
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

            lexeme &plugin:: inject(token &word)  
            {
                if(word.size<=0) throw exception("<%s> returned empty", (*label)());
                lexeme *lx = newlex(label,*word);
                (**lx).swap_with(word);
                root->store(lx);
                return *lx;
            }
            

        }

    }
}
