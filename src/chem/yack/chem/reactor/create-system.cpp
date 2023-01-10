#include "yack/chem/reactor/cluster.hpp"

namespace yack
{
    namespace chemical
    {
        
        static inline
        bool are_all_unbounded(const actors   &players,
                               const addrbook &tribe) throw()
        {
            for(const actor *a=players->head;a;a=a->next)
            {
                const species &s = **a;
                if( !tribe.search(&s) ) return false;
            }
            return true;
        }

        static inline
        bool is_special(const equilibrium &eq, const addrbook &tribe) throw()
        {
            return are_all_unbounded(eq.reac,tribe) || are_all_unbounded(eq.prod,tribe);
        }


        void cluster:: create_system()
        {

            eq_group  remaining;

            eq_group & roaming = coerce( *(genus->roaming) );
            eq_group & bounded = coerce( *(genus->bounded) );
            eq_group & special = coerce( *(genus->special) );

            //------------------------------------------------------------------
            // first pass: collect obvious roaming eqs and species
            //------------------------------------------------------------------
            addrbook   tribe;
            for(const gnode *gn = (*group)->head; gn; gn=gn->next )
            {
                const equilibrium &eq = gn->host;
                if(eq.reac->size<=0 || eq.prod->size<=0)
                {
                    roaming << *gn;
                    eq.update(tribe);
                }
                else
                {
                    remaining  << *gn;
                }
            }

            //------------------------------------------------------------------
            // second pass: processing remaining to detect more unbounded
            //------------------------------------------------------------------
            {
            SECOND_PASS:
                for(const eq_gnode *node=remaining.head;node;node=node->next)
                {
                    const equilibrium &eq = (***node).host;
                    if(is_special(eq,tribe))
                    {
                        if(eq.update(tribe))
                        {
                            // updated list of species
                            goto SECOND_PASS;
                        }
                    }
                }
            }


            //------------------------------------------------------------------
            // third pass: dispatch remaining
            //------------------------------------------------------------------
            while( remaining.size )
            {
                eq_gnode          *gn = remaining.pop_front();
                const equilibrium &eq = (***gn).host;
                if(is_special(eq,tribe))
                    special.push_back(gn);
                else
                    bounded.push_back(gn);
            }

            //------------------------------------------------------------------
            // finalize: dispatch species
            //------------------------------------------------------------------
            for(const anode *sn = (*alive)->head; sn; sn=sn->next )
            {
                const species &s = sn->host;
                if( tribe.search(&s) )
                {
                    coerce( *(breed->unbounded) ) << *sn;
                }
                else
                {
                    coerce( *(breed->conserved) ) << *sn;
                }
            }


        }


    }

}

