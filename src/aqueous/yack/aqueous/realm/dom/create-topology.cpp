#include "yack/aqueous/realm/domain.hpp"
#include "yack/system/imported.hpp"
#include "yack/apex/alga.hpp"

namespace yack
{
    namespace aqueous
    {

        static inline
        void update_alpha(matrix<int>    &alpha,
                          const eq_list  &roaming,
                          const addrbook &sdb) noexcept
        {
            for(const eq_node *en=roaming.head;en;en=en->next)
            {
                const equilibrium &eq = ***en;
                alpha[ eq.indx[1] ].ld(0);
            }

            for(addrbook::const_iterator it=sdb.begin();it!=sdb.end();++it)
            {
                const size_t j = static_cast<const species *>( *it )->indx[1];
                for(size_t i=alpha.rows;i>0;--i) alpha[i][j] = 0;
            }
        }

       


        void domain:: create_topology(const xmlog &xml)
        {
          
            //addrbook sdb;


            //------------------------------------------------------------------
            // compute topology matrix
            //------------------------------------------------------------------
            {
                matrix<int> &Nu_ = coerce(Nu);
                Nu_.make(N,M);
                for(const eq_node *en=head;en;en=en->next)
                {
                    const equilibrium &eq = ***en;
                    const size_t       ei = eq.indx[1];
                    writable<int>     &nu = Nu_[ei];
                    eq.fill(nu,1);
#if 0
                    if(eq.prod.size<=0 || eq.reac.size<=0)
                    {
                        assert(is_roaming(nu));
                        coerce(roaming) << eq;
                        eq.report_to(sdb);   //!< register roaming specie(s)
                    }
                    else
                    {
                        coerce(defined) << eq;
                    }
#endif
                }
            }

            {
                const size_t rank = alga::rank(Nu);
                if( rank < N ) throw imported::exception(clid,"rank=%u < %u, check dependencies!!", unsigned(rank), unsigned(N) );
            }

            //------------------------------------------------------------------
            // deduce transposed
            //------------------------------------------------------------------
            coerce(NuT).make(M,N);
            coerce(NuT).assign(Nu,transposed);
            YACK_XMLOG(xml, "Nu   = " << Nu);

#if 0
            //------------------------------------------------------------------
            // initialize alpha
            //------------------------------------------------------------------
            alpha.make(N,M);
            alpha.assign(Nu);

        CYCLE:
            {
                update_alpha(alpha,roaming,sdb);
                YACK_XMLOG(xml,"alpha= " << alpha);

                eq_list keep;
                eq_list roam;
                while(defined.size>0)
                {
                    eq_node           * en = coerce(defined).pop_front();
                    const equilibrium & eq = ***en;
                    if( is_roaming( alpha[ eq.indx[1] ]) )
                    {
                        roam.push_back(en);
                        eq.report_to(sdb);
                    }
                    else
                    {
                        keep.push_back(en);
                    }
                }
                coerce(defined).swap_with(keep);
                if(roam.size)
                {
                    coerce(roaming).merge_back(roam);
                    goto CYCLE;
                }
            }

            for(const sp_node *sn=live.head;sn;sn=sn->next)
            {
                const species &sp = ***sn;
                if(sdb.search(&sp))
                {
                    coerce(endless) << sp;
                }
                else
                {
                    coerce(bounded) << sp;
                }
            }

            YACK_XMLOG(xml,"--> roaming = " << roaming);
            YACK_XMLOG(xml,"--> defined = " << defined);
            YACK_XMLOG(xml,"--> bounded = " << bounded);
            YACK_XMLOG(xml,"--> endless = " << endless);
#endif
        }

    }

}

