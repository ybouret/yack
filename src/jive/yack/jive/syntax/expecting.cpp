
#include "yack/jive/syntax/expecting.hpp"
#include "yack/jive/syntax/rule/terminal.hpp"

namespace yack
{

    namespace jive
    {

        namespace syntax
        {

            expected:: expected(const terminal &t) throw() :
            object(), authority<const terminal>(t), next(0), prev(0)
            {
            }

            expected:: ~expected() throw()
            {
            }
            

        }

    }

}

