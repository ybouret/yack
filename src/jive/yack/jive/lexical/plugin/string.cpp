
#include "yack/jive/lexical/plugin/string.hpp"

namespace yack
{
    namespace jive
    {

        namespace lexical
        {
            string_:: ~string_() throw()
            {

            }

            void string_:: setup()
            {
                back(eos,this, &string_::leave);
            }

            void string_:: enter(token &t) throw()
            {
                // token is the 'init' single char
                assert(1==t.size);
                content.release();
                content.swap_with(t);
            }

            void string_:: leave(token &t)
            {
                // token is the 'done' single char
                assert(1==t.size);

                // merge to finish string
                content.merge_back(t);

                // create a new string, inject into analyzer
                inject(content);
            }


        }

    }

}

