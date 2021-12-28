#include "yack/jive/syntax/rule/compound/alternate.hpp"
#include "yack/jive/syntax/xnode.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            alternate:: ~alternate() throw()
            {
            }

            alternate:: alternate(const tag &name_) throw() :
            compound(name_,mark)
            {
                I_am<alternate>();
            }

            bool alternate:: accept(YACK_JIVE_RULE_ARGS) const
            {
                bool result = false;

                for(const component *chld=head;chld;chld=chld->next)
                {
                    xnode *here = NULL;
                    if( (**chld).accept(src,lxr,here) )
                    {
                        result = true;
                        if(here)
                        {
                            xnode::grow(tree,here);
                            return true;
                        }
                        // else leave a chance to another component,
                        // but result will be true
                    }
                }
                
                return result;
            }

        }

    }

}


