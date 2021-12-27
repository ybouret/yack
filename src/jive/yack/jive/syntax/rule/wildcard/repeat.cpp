#include "yack/jive/syntax/rule/wildcard/repeat.hpp"
#include "yack/jive/syntax/xnode.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {
            repeat:: ~repeat() throw()
            {
            }
            
            repeat:: repeat(const tag      &name_,
                            const rule     &host_,
                            const size_t    nmin) :
            wildcard(name_,mark,host_),
            count(nmin)
            {
                I_am<repeat>();
            }

            bool repeat:: accept(YACK_JIVE_RULE_ARGS) const
            {
                xnode           *sub = xnode::make(*this);
                size_t           num = 0;

                {
                    const  rule     &you = **this;
                    auto_ptr<xnode>  ptr = sub;
                    while(you.accept(src,lxr,sub))
                    {
                        ++num;
                    }
                    ptr.dismiss();
                }

                if(num>=count)
                {
                    xnode::grow(tree,sub);
                    return true;
                }
                else
                {
                    xnode::ret(lxr,sub);
                    return false;
                }
            }

        }
        
    }
}

