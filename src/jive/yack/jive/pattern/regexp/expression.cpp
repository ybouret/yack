#include "yack/jive/pattern/regexp/compiler.hpp"
#include "yack/jive/pattern/all.hpp"

namespace yack
{
    namespace jive
    {

        pattern * rx_compiler:: new_expression()
        {
            auto_ptr<logical> p = new op_and();

            while(curr<last)
            {
                const char c = *(curr++);

                switch(c)
                {
                    default:
                        p->push_back( new single(c) );
                }

            }


            return pattern::optimize( p.yield() );
        }

    }

}

