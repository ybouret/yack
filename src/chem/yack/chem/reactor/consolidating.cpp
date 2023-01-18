#include "yack/chem/reactor/cluster.hpp"
#include "yack/system/imported.hpp"

namespace yack
{

    namespace chemical
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
                        reac_only << *node;
                        eq.update(nomadic);
                        roaming.ensure(&eq);
                    }
                    else
                    {
                        if(eq.prod->size<=0)
                        {
                            prod_only << *node;
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
                for(const glist::node_type *node=grp->head;node;node=node->next)
                {
                    const equilibrium &eq = node->host; if(roaming.search(&eq)) continue;
                    if( has_conserved_species(eq.reac,nomadic) && has_conserved_species(eq.prod,nomadic) )
                    {
                        delimited << *node;
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

