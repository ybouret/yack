#include "yack/jive/syntax/rule/terminal.hpp"
#include "yack/jive/syntax/xnode.hpp"

namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            terminal:: ~terminal() throw()
            {
            }

            terminal:: terminal(const tag &t, const term_role r) throw() :
            rule(t,terminal_type,mark),
            role(r)
            {
                I_am<terminal>();
            }


            bool terminal:: accept(YACK_JIVE_RULE_ARGS) const
            {
                lexeme *lx = lxr.query(src);
                if(verbose) std::cerr << "?term <" << name << ">" << std::endl;
                if(lx)
                {
                    if( *(lx->name) == *(this->name) )
                    {
                        xnode::grow(tree,*this,lx);
                        if(verbose)
                        {
                            std::cerr << "+term <" << name << "> = '" << **lx << "'" << std::endl;
                        }
                        return true;
                    }
                    else
                    {
                        if(verbose)
                        {
                            std::cerr << "-term <" << name << "> but <" << lx->name << ">" << std::endl;
                        }
                        lxr.store(lx);
                        return false;
                    }
                }
                else
                {
                    if(verbose) std::cerr << "-term <" << name << "> but EOS" << std::endl;
                    // EOF
                    return false;
                }
            }
            
        }
    }
}

