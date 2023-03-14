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

        static inline bool is_roaming(const readable<int> &nu) noexcept
        {
            static const unsigned NONE = 0x00;
            static const unsigned REAC = 0x01;
            static const unsigned PROD = 0x02;
            static const unsigned BOTH = REAC | PROD;

            unsigned flag = NONE;
            for(size_t j=nu.size();j>0;--j)
            {
                switch( __sign::of(nu[j]) )
                {
                    case __zero__: continue;
                    case positive: flag |= PROD; break;
                    case negative: flag |= REAC; break;
                }
            }

            switch(flag)
            {
                case REAC:
                case PROD:
                case NONE:
                    return true;

                case BOTH:
                    break;
            }
            assert(BOTH==flag);
            return false;
        }


        void domain:: create_topology(const xmlog &xml)
        {
            eq_list  roaming;
            eq_list  defined;
            sp_list  endless;
            sp_list  limited;
            addrbook sdb;


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
                    if(eq.prod.size<=0 || eq.reac.size<=0)
                    {
                        assert(is_roaming(nu));
                        roaming << eq;
                        eq.report_to(sdb);   //!< register roaming specie(s)
                    }
                    else
                    {
                        defined << eq;
                    }

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

            //------------------------------------------------------------------
            // initialize alpha
            //------------------------------------------------------------------
            matrix<int> alpha(Nu);

        CYCLE:
            {
                update_alpha(alpha,roaming,sdb);
                YACK_XMLOG(xml,"alpha=" << alpha);
                eq_list keep;
                eq_list roam;
                while(defined.size>0)
                {
                    eq_node           * en = defined.pop_front();
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
                defined.swap_with(keep);
                if(roam.size)
                {
                    roaming.merge_back(roam);
                    goto CYCLE;
                }
            }

            for(const sp_node *sn=live.head;sn;sn=sn->next)
            {
                const species &sp = ***sn;
                if(sdb.search(&sp))
                {
                    endless << sp;
                }
                else
                {
                    limited << sp;
                }
            }

            YACK_XMLOG(xml,"--> roaming = " << roaming);
            YACK_XMLOG(xml,"--> defined = " << defined);
            YACK_XMLOG(xml,"--> limited = " << limited);
            YACK_XMLOG(xml,"--> endless = " << endless);


            exit(0);

        }

    }

}

