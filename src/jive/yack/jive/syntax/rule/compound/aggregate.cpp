

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

                if(verbose) std::cerr << "?agg  <" << name << ">" << std::endl;
                //--------------------------------------------------------------
                //
                // try to accept all components
                //
                //--------------------------------------------------------------
                for(const component *chld=head;chld;chld=chld->next)
                {
                    if(! (**chld).accept(src,lxr,here,obs))
                    {
                        keep.dismiss();       // dismiss guard
                        xnode::ret(lxr,here); // return sub tree
                        if(verbose)
                        {
                            std::cerr << "-agg  <" << name << ">" << std::endl;
                        }
                        return false;
                    }
                }

                // all done
                if(verbose) std::cerr << "+agg  <" << name << "> with #" << here->sub().size << std::endl;
                keep.dismiss();
                switch(role)
                {
                    case entity: xnode::grow(tree,here); break;
                    case acting: xnode::join(tree,here); break;
                }
                return true;
            }
            


        }

    }

}


