
#include "yack/jive/lexical/plugin.hpp"

namespace yack
{
    namespace jive
    {

        namespace lexical
        {
            plugin:: ~plugin() throw()
            {}


            void plugin:: enter(token &word)
            {
                initialize(word);
            }

        }

    }
}
