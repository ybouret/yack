
#include "yack/aqueous/realm/domain.hpp"
#include "yack/system/imported.hpp"
//#include "yack/apex/alga.hpp"


namespace yack
{
    namespace aqueous
    {

        static const unsigned NONE = 0x00;
        static const unsigned REAC = 0x01;
        static const unsigned PROD = 0x02;
        static const unsigned BOTH = REAC | PROD;

        static inline unsigned category_of(const readable<int> &nu) noexcept
        {
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
            return flag;
        }

        static inline unsigned category_of(const equilibrium &eq) noexcept
        {
            unsigned flag = NONE;
            if(eq.reac.size>0) flag |= REAC;
            if(eq.prod.size>0) flag |= PROD;
            return flag;
        }

        static inline
        bool is_roaming(const readable<int> &nu) noexcept
        {

            const unsigned flag = category_of(nu);
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



        void domain:: make_categories(const xmlog &xml)
        {
            YACK_XMLSUB(xml,"make_categories");

            //------------------------------------------------------------------
            //
            // create global topology
            //
            //------------------------------------------------------------------
            matrix<int> alpha(L,M);
            addrbook    eq_roam;
            addrbook    sp_roam;
            {
                size_t      count = 1;
                for(const eq_node *en=head;en;en=en->next,++count)
                {
                    const equilibrium &eq = ***en;assert(count==eq.indx[sub_level]);
                    writable<int>     &nu = alpha[count];
                    eq.fill(nu,1);
                    if(is_roaming(nu))
                    {
                        eq_roam.ensure(&eq);
                        eq.report_to(sp_roam);
                    }
                }
            }


        UPDATE_ALPHA:
            for(addrbook::const_iterator it=eq_roam.begin();it!=eq_roam.end();++it)
            {
                const equilibrium &eq = *static_cast<const equilibrium *>(*it);
                const size_t       ei = eq.indx[sub_level];
                alpha[ei].ld(0);
            }

            for(addrbook::const_iterator it=sp_roam.begin();it!=sp_roam.end();++it)
            {
                const species &sp = *static_cast<const species *>(*it);
                const size_t   sj = sp.indx[sub_level];
                for(size_t i=L;i>0;--i)
                {
                    alpha[i][sj] = 0;
                }
            }


            const size_t nroam = eq_roam->size;
            for(const eq_node *en=head;en;en=en->next)
            {
                const equilibrium &eq = ***en;
                if(is_roaming(alpha[eq.indx[sub_level]]))
                {
                    eq_roam.ensure(&eq);
                    eq.report_to(sp_roam);
                }
            }
            if(eq_roam->size>nroam)
                goto UPDATE_ALPHA;

            //------------------------------------------------------------------
            //
            // dispatching all equilibria
            //
            //------------------------------------------------------------------
            {
                size_t idef = 1;
                size_t ispl = 1;
                size_t icmb = 1;
                size_t ioth = 1;
                for(const eq_node *en=head;en;en=en->next)
                {
                    const equilibrium &eq = ***en;
                    if(eq_roam.search(&eq))
                    {
                        switch(category_of(eq))
                        {
                            case NONE:
                            case BOTH:
                                coerce(roaming) << eq;
                                coerce(eq.indx[cat_level]) = ioth++;
                                break;
                                
                            case PROD:
                                coerce(splitting) << eq;
                                coerce(eq.indx[cat_level]) = ispl++;
                                break;
                                
                            case REAC:
                                coerce(combining) << eq;
                                coerce(eq.indx[cat_level]) = icmb++;
                                break;
                        }
                    }
                    else
                    {
                        coerce(defined) << eq;
                        coerce(eq.indx[cat_level]) = idef++;
                    }
                }
            }

            //------------------------------------------------------------------
            //
            // dispatching all species
            //
            //------------------------------------------------------------------
            for(const sp_node *sn=live.head;sn;sn=sn->next)
            {
                const species &sp = ***sn;
                if(sp_roam.search(&sp))
                {
                    coerce(endless) << sp;
                    coerce( reg[ sp.indx[top_level]] ) = false;
                }
                else
                {
                    coerce(bounded) << sp;
                }
            }


            YACK_XMLOG(xml,"-------- equilibria --------");
            YACK_XMLOG(xml,"splitting : " << splitting);
            YACK_XMLOG(xml,"combining : " << combining);
            YACK_XMLOG(xml,"->roaming : " << roaming);
            YACK_XMLOG(xml,"->defined : " << defined);

            YACK_XMLOG(xml,"--------   species  --------");
            YACK_XMLOG(xml,"bounded     : " << bounded);
            YACK_XMLOG(xml,"endless     : " << endless);



        }
    }

}

