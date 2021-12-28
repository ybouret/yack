

#include "yack/jive/syntax/rule/compound/aggregate.hpp"
#include "yack/jive/syntax/xnode.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            aggregate:: ~aggregate() throw()
            {
            }

            aggregate:: aggregate(const tag &name_) throw() :
            compound(name_,mark)
            {
            }

            bool aggregate:: accept(YACK_JIVE_RULE_ARGS) const
            {
                xnode          *here = xnode::make(*this);
                auto_ptr<xnode> keep = here;

                // try to accept all components
                for(const component *chld=head;chld;chld=chld->next)
                {
                    const rule &r = **chld;
                    if(!r.accept(src,lxr,here))
                    {
                        keep.dismiss();
                        xnode::ret(lxr,here);
                        return false;
                    }
                }

                // done
                keep.dismiss();
                xnode::grow(tree,here);
                return true;
            }
            


        }

    }

}


