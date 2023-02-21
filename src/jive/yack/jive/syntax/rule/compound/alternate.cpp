#include "yack/jive/syntax/rule/compound/alternate.hpp"
#include "yack/jive/syntax/xnode.hpp"
#include "yack/type/temporary.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            alternate:: ~alternate() noexcept
            {
            }

            alternate:: alternate(const tag &name_) noexcept :
            compound(name_,mark)
            {
                I_am<alternate>();
            }

            bool alternate:: accept(YACK_JIVE_RULE_ARGS) const
            {
                bool result = false;
                YACK_JIVE_SYN_PRINTLN(obs << "?alt  <" << name << ">");
                {
                    const temporary_increase<unsigned> incr(obs.depth);
                    for(const component *chld=head;chld;chld=chld->next)
                    {
                        xnode *here = NULL;
                        if( (**chld).accept(src,lxr,here,obs) )
                        {
                            result = true;
                            if(here)
                            {
                                --obs.depth;
                                YACK_JIVE_SYN_PRINTLN(obs << "+alt  <" << name << "> as <" << (**here).name << ">");
                                xnode::grow(tree,here);
                                return true;
                            }
                            // else leave a chance to another component,
                            // but result will be true
                        }
                    }
                }

                if(verbose)
                {
                    if(result) std::cerr << obs << "+alt  <" << name << "> EMPTY" << std::endl;
                    else       std::cerr << obs << "-alt  <" << name << ">"       << std::endl;
                }
                return result;
            }

        }

    }

}


