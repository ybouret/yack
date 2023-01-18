#include "yack/chem/reactor/cluster.hpp"
#include "yack/system/imported.hpp"

#include <cstring>

namespace yack
{

    namespace chemical
    {

        namespace
        {
            static inline
            bool has_conserved_species(const actors   &A,
                                       const addrbook &nomadic) throw()
            {
                for(const actor *a=A->head;a;a=a->next)
                {
                    const species &s = **a;
                    if( !nomadic.search(&s) ) return true;
                }
                return false;
            }


            static inline
            bool has_roaming(const equilibrium &lhs, const addrbook &roaming) throw()
            {
                const string &lid = lhs.name;
                std::cerr << "\t\t Checking " << lid << std::endl;
                for(addrbook::const_iterator it=roaming.begin();it!=roaming.end();++it)
                {
                    const equilibrium &rhs = *static_cast<const equilibrium *>(*it);
                    const string      &rid = rhs.name;
                    if(rid.size()<=lid.size() && NULL!=strstr(lid(),rid()) ) return true;
                }
                return false;
            }
        }

        void cluster:: consolidating(const xmlog &xml)
        {
            YACK_XMLSUB(xml,"cluster::consolidating");

            const glist &grp = *group;
            const alist &act = *alive;

            addrbook     roaming; // equilibria
            addrbook     nomadic; // species

            //------------------------------------------------------------------
            //
            // finding reac_only/prod_only
            //
            //------------------------------------------------------------------
            {
                eq_group    &reac_only = coerce(*(replica->genus->reac_only));
                eq_group    &prod_only = coerce(*(replica->genus->prod_only));
                for(const glist::node_type *node=grp->head;node;node=node->next)
                {
                    const equilibrium &eq = node->host;
                    if(eq.reac->size<=0)
                    {
                        if(eq.prod->size<=0) throw imported::exception(clid,"empty <%s>", eq.name() );
                        prod_only << *node;
                        eq.update(nomadic);
                        roaming.ensure(&eq);
                    }
                    else
                    {
                        if(eq.prod->size<=0)
                        {
                            reac_only << *node;
                            eq.update(nomadic);
                            roaming.ensure(&eq);
                        }
                    }
                }


            }

            //------------------------------------------------------------------
            //
            // updating from nomadic
            //
            //------------------------------------------------------------------
            {
                sp_group &unbounded = coerce( *(replica->breed->unbounded) );
                sp_group &conserved = coerce( *(replica->breed->conserved) );

                for(const anode *node=act->head;node;node=node->next)
                {
                    const species &sp = node->host;
                    if(nomadic.search(&sp))
                    {
                        unbounded << *node;
                        coerce(*fixed)[ *sp ] = false;
                    }
                    else
                    {
                        conserved << *node;
                        assert(true==(*fixed)[*sp]);
                    }
                }
            }



            {
                eq_group    &delimited = coerce(*(replica->genus->delimited));
                eq_group    &undefined = coerce(*(replica->genus->undefined));
                eq_repo_    &balancing = coerce(*(replica->genus->balancing));

                for(const glist::node_type *node=grp->head;node;node=node->next)
                {
                    const equilibrium &eq = node->host; if(roaming.search(&eq)) continue;
                    if( has_conserved_species(eq.reac,nomadic) && has_conserved_species(eq.prod,nomadic) )
                    {
                        delimited << *node;
                        if(!has_roaming(eq,roaming))
                            balancing << eq;
                    }
                    else
                    {
                        undefined << *node;
                    }
                }
            }


            replica->display(xml, "replica");

        }
    }

}

