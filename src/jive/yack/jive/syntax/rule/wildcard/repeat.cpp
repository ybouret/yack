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
                            const size_t    nmin) throw() :
            wildcard(name_,mark,host_),
            count(nmin)
            {
                I_am<repeat>();
            }

            bool repeat:: accept(YACK_JIVE_RULE_ARGS) const
            {
                xnode           *here = xnode::make(*this);
                size_t           nrep = 0;

                {
                    const  rule     &that = **this;
                    auto_ptr<xnode>  keep = here;
                    while(that.accept(src,lxr,here,obs))
                    {
                        ++nrep;
                    }
                    keep.dismiss();
                }

                if(nrep>=count)
                {
                    xnode::join(tree,here);
                    return true;
                }
                else
                {
                    xnode::ret(lxr,here);
                    return false;
                }
            }

        }
        
    }
}

