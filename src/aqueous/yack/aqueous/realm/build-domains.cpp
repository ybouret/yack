#include "yack/aqueous/realm.hpp"
#include "yack/data/list/sort.hpp"

namespace yack
{
    namespace aqueous
    {
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
                YACK_XMLOG(xml,"--> " << eq);

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
                    push_back( new domain(eq,reg,grp) );
                    YACK_XMLOG(xml,"==> " << (*tail));
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
                                YACK_XMLOG(xml,"==> " << *lhs << " + " << *rhs);
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

            // post-processing
            for(domain *dom=head;dom;dom=dom->next)
                merge_list_of<eq_node>::sort(*dom, domain::compare);
            
            YACK_XMLOG(xml,"#domains = " << size);
            YACK_XMLOG(xml, *this);


         }

    }
}

