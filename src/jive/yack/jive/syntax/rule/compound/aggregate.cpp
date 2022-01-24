

#include "yack/jive/syntax/rule/compound/aggregate.hpp"
#include "yack/jive/syntax/xnode.hpp"
#include "yack/type/temporary.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            aggregate:: ~aggregate() throw()
            {
            }

            aggregate:: aggregate(const tag     &name_,
                                  const agg_role role_) throw() :
            compound(name_,mark),
            role(role_)
            {
                I_am<aggregate>();
            }

            bool aggregate:: accept(YACK_JIVE_RULE_ARGS) const
            {
                xnode          *here = xnode::make(*this);
                auto_ptr<xnode> keep = here;

                YACK_JIVE_SYN_PRINTLN(obs << "?agg  <" << name << ">");
                //--------------------------------------------------------------
                //
                // try to accept all components
                //
                //--------------------------------------------------------------
                {
                    const temporary_increase<unsigned> incr(obs.depth);
                    for(const component *chld=head;chld;chld=chld->next)
                    {
                        if(! (**chld).accept(src,lxr,here,obs))
                        {
                            keep.dismiss();       // dismiss guard
                            xnode::ret(lxr,here); // return sub tree
                            --obs.depth;
                            YACK_JIVE_SYN_PRINTLN(obs << "-agg  <" << name << ">");
                            return false;
                        }
                    }
                }

                // all done
                YACK_JIVE_SYN_PRINTLN(obs << "+agg  <" << name << "> with #" << here->size());
                if(here->size()>0)
                {
                    keep.dismiss();
                    switch(role)
                    {
                        case named: xnode::grow(tree,here); break;
                        case group: xnode::join(tree,here); break;
                        case proxy:
                            switch(here->size())
                            {
                                case 0:  delete here; break;
                                case 1:  xnode::join(tree,here); break;
                                default: xnode::grow(tree,here); break;
                            }
                            break;
                    }
                }
                // else here is deleted...
                return true;
            }
            


        }

    }

}


