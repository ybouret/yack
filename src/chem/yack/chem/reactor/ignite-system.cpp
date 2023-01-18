#include "yack/chem/reactor/cluster.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace chemical
    {
        


        void cluster:: ignite_system()
        {

            eq_group & delimited = coerce( *(primary->genus->delimited) );
            eq_group & reac_only = coerce( *(primary->genus->reac_only) );
            eq_group & prod_only = coerce( *(primary->genus->prod_only) );

            //------------------------------------------------------------------
            // first pass: obvious classification, collect unboundned
            //------------------------------------------------------------------
            addrbook   tribe;
            for(const gnode *gn = (*group)->head; gn; gn=gn->next )
            {
                const equilibrium &eq = gn->host;
                if(eq.reac->size<=0)
                {
                    if(eq.prod->size<=0)
                    {
                        throw imported::exception(clid,"undefined <%s>", eq.name() );
                    }
                    else
                    {
                        prod_only << *gn;
                        eq.update(tribe);
                    }
                }
                else
                {
                    if(eq.prod->size<=0)
                    {
                        reac_only << *gn;
                        eq.update(tribe);
                    }
                    else
                    {
                        delimited << *gn;
                    }
                }

            }

            
            //------------------------------------------------------------------
            // second pass: dispatch species
            //------------------------------------------------------------------
            for(const anode *sn = (*alive)->head; sn; sn=sn->next )
            {
                const species &s = sn->host;
                if( tribe.search(&s) )
                {
                    coerce( *(primary->breed->unbounded) ) << *sn;
                }
                else
                {
                    coerce( *(primary->breed->conserved) ) << *sn;
                }
            }
            
        }


    }

}

