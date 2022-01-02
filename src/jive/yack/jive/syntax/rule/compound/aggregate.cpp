

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
                xnode::grow(tree,here);
                return true;
            }
            


        }

    }

}


