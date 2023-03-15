
#include "yack/aqueous/realm/domain.hpp"
#include "yack/system/imported.hpp"
//#include "yack/apex/alga.hpp"


namespace yack
{
    namespace aqueous
    {

        static inline
        bool is_roaming(const readable<int> &nu) noexcept
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



        void domain:: make_categories(const xmlog &xml)
        {
            YACK_XMLSUB(xml,"make_categories");
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
                    if(eq.reac.size<=0 && eq.prod.size>0 ) coerce(splitting) << eq;
                    if(eq.reac.size>0  && eq.prod.size<=0) coerce(combining) << eq;
                }
            }

            //YACK_XMLOG(xml,"alpha=" << alpha);

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

            //YACK_XMLOG(xml,"alpha=" << alpha);

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

            // dispatching all equilibria
            for(const eq_node *en=head;en;en=en->next)
            {
                const equilibrium &eq = ***en;
                if(eq_roam.search(&eq))
                {
                    coerce(roaming) << eq;
                }
                else
                {
                    coerce(defined) << eq;
                }
            }

            for(const sp_node *sn=live.head;sn;sn=sn->next)
            {
                const species &sp = ***sn;
                if(sp_roam.search(&sp))
                {
                    coerce(endless) << sp;
                }
                else
                {
                    coerce(bounded) << sp;
                }
            }

            YACK_XMLOG(xml,"roaming: " << roaming);
            YACK_XMLOG(xml,"defined: " << defined);

            YACK_XMLOG(xml,"endless: " << endless);
            YACK_XMLOG(xml,"bounded: " << bounded);



        }
    }

}

