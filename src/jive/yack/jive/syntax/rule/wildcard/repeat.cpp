#include "yack/jive/syntax/rule/wildcard/repeat.hpp"
#include "yack/jive/syntax/xnode.hpp"
#include "yack/type/temporary.hpp"

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
                const  rule     &that = **this;

                {
                    auto_ptr<xnode>  keep = here;
                    YACK_JIVE_SYN_PRINTLN( obs << "?rep(" << that.name << ")>=" << count);
                    const temporary_increase<unsigned> incr(obs.depth);
                    while(that.accept(src,lxr,here,obs))
                    {
                        ++nrep;
                    }
                    keep.dismiss();
                }

                if(nrep>=count)
                {
                    YACK_JIVE_SYN_PRINTLN(obs << "+rep(" << that.name << ") #" << nrep);
                    xnode::join(tree,here);
                    return true;
                }
                else
                {
                    YACK_JIVE_SYN_PRINTLN(obs << "-rep(" << that.name << ") #" << nrep << "<" << count);
                    xnode::ret(lxr,here);
                    return false;
                }
            }

        }
        
    }
}

