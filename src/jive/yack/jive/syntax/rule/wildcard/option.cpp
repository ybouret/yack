#include "yack/jive/syntax/rule/wildcard/option.hpp"
#include "yack/jive/syntax/xnode.hpp"
#include "yack/type/temporary.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            option:: ~option() throw()
            {
            }
            
            option:: option(const tag      &name_,
                            const rule     &host_) throw():
            wildcard(name_,mark,host_)
            {
                I_am<option>();
            }

            bool option:: accept(YACK_JIVE_RULE_ARGS) const
            {
                const rule &r    = **this;
                xnode      *node = NULL;
                YACK_JIVE_SYN_PRINTLN(obs<<"?opt(" << r.name << ")");
                const temporary_increase<unsigned> incr(obs.depth);
                if( r.accept(src,lxr,node,obs) )
                {
                    xnode::grow(tree,node);
                }
                return true;
            }
            
        }
        
    }
}

