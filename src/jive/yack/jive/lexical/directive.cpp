
#include "yack/jive/lexical/directive.hpp"

namespace yack
{
    namespace jive
    {
        namespace lexical
        {
            directive:: ~directive() throw()
            {
            }

            directive:: directive(const tag    &t,
                                  const motif  &m,
                                  const action &a) :
            object(),
            counted(),
            uuid(t),
            info(m),
            duty(a)
            {
            }

            const string & directive:: key() const throw()
            {
                return *uuid;
            }

            
        }

    }
}


