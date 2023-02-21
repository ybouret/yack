
#include "yack/jive/lexical/directive.hpp"

namespace yack
{
    namespace jive
    {
        namespace lexical
        {
            directive:: ~directive() noexcept
            {
            }

            directive:: directive(const tag    &t,
                                  const motif  &m,
                                  const action &a) :
            object(),
            counted(),
            name(t),
            info(m),
            duty(a)
            {
            }

            const string & directive:: key() const noexcept
            {
                return *name;
            }

            
        }

    }
}


