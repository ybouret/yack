
#include "yack/aqueous/realm.hpp"

namespace yack
{
    namespace aqueous
    {
        realm:: ~realm() noexcept
        {
        }

        realm:: realm(const library    &lib_,
                      const equilibria &eqs_) :
        domains(),
        lib(lib_),
        eqs(eqs_)
        {
            xmlog xml("[realm]",std::cerr,species::verbose);
            build_domains(xml);
        }

        void realm:: build_domains(const xmlog &xml)
        {
            YACK_XMLSUB(xml,"build_domains");
            for(const enode *en=eqs->head;en;en=en->next)
            {
                //--------------------------------------------------------------
                // get current equilibrium
                //--------------------------------------------------------------
                const equilibrium &eq = ***en;
                bool               ok = false;
                YACK_XMLOG(xml,"+ <" << eq.name << ">");

                //--------------------------------------------------------------
                // look for an accepting domain
                //--------------------------------------------------------------
                for(domain *dom=head;dom;dom=dom->next)
                {
                    if(dom->accepts(eq))
                    {
                        (*dom) << eq;
                        ok = true;
                        break;
                    }
                }

                if(!ok)
                {
                    //----------------------------------------------------------
                    // create new domain
                    //----------------------------------------------------------
                    push_back( new domain(eq) );
                }
                else
                {
                    //----------------------------------------------------------
                    // check for fusion
                    //----------------------------------------------------------
                    domains doms;
                    while(size)
                    {
                        domain *rhs = pop_front();
                        for(domain *lhs=doms.head;lhs;lhs=lhs->next)
                        {
                            if(lhs->accepts(*rhs))
                            {
                                lhs->merge_back(*rhs);
                                delete rhs;
                                rhs = NULL;
                                break;
                            }
                        }

                        if(rhs)
                            doms.push_back(rhs);
                    }
                    swap_with(doms);
                }

            }
            YACK_XMLOG(xml,"#domains = " << size);
            std::cerr << *this << std::endl;
        }

    }
    
}

