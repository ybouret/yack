
#include "yack/chem/nexus.hpp"
#include "yack/math/iota.hpp"
#include "yack/raven/qbranch.hpp"
#include "yack/math/algebra/ortho-family.hpp"
#include "yack/apex/kernel.hpp"

#include <iomanip>

namespace yack
{
    using namespace math;
    using namespace raven;

    namespace chemical
    {


        void nexus:: conserved_set_(cluster &sharing, const xmlog &xml)
        {
            static const char * const fn = "conserved_subset";
            YACK_XMLSUB(xml,fn);
            eq_team      usual;
            sp_list      house;

            //------------------------------------------------------------------
            //
            // extract usual equilibria and house of species
            //
            //------------------------------------------------------------------
            {
                addrbook     tribe;

                for(const eq_node *en=sharing.head;en;en=en->next)
                {
                    const equilibrium &eq = **en;
                    if( regular.met(eq) )
                    {
                        usual << &eq;
                        eq.update(tribe);
                    }
                }

                for(addrbook::const_iterator it=tribe.begin();it!=tribe.end();++it)
                {
                    const species &sp = *static_cast<species*>(*it);
                    if( conserved != crit[*sp] ) continue;;
                    house << static_cast<species *>(*it);
                }
                house.sort();
            }

            //------------------------------------------------------------------
            //
            // study result
            //
            //------------------------------------------------------------------
            YACK_XMLOG(xml, "-- #usual = " << usual.size << " / " << sharing.size << ", #house=" << house.size);
            if(usual.size<=0) return;
            if(verbose)
            {
                for(const eq_node *node=usual.head;node;node=node->next)
                {
                    *xml << " (*) " << (**node).name << std::endl;
                }
                *xml << "  |_" << house << std::endl;
            }
            assert(house.size>=2);


            const size_t n = usual.size;
            const size_t m = house.size;
            switch( __sign::of(m,n) )
            {
                case negative: throw imported::exception(fn,"too few species");
                case __zero__:
                    YACK_XMLOG(xml, "-- specific system");
                    return;
                case positive:
                    break;
            }

            //------------------------------------------------------------------
            //
            // build virtual topology
            //
            //------------------------------------------------------------------
            assert(n<m);
            imatrix nu(n,m);
            {
                size_t i=1;
                for(const eq_node *en=usual.head;en;en=en->next,++i)
                {
                    const readable<int> &Nu_i = Nu[***en];
                    writable<int>       &nu_i = nu[i];
                    size_t j=1;
                    for(const sp_node *sn=house.head;sn;sn=sn->next,++j) {
                        nu_i[j] = Nu_i[ ***sn ];
                    }
                }
            }
            std::cerr << "nu=" << nu << std::endl;

            matrix<apz> Q(m,m);
            if(!ortho_family::build(Q,nu)) throw imported::exception(fn,"singular sub-system");
            std::cerr << "Q=" << Q << std::endl;

            raven::qmatrix F(m,m);

            // build Span(nu)
            for(size_t i=1;i<=n;++i)
            //for(size_t i=n;i>0;--i)
            {
                if(!F(nu[i])) throw imported::exception(fn,"direct span RAVEn failure");
                std::cerr << "F=" << F << std::endl;
            }

            // build Span(nu_ortho)
            assert(qmatrix::fully_grown != F.active_state);
            //            for(size_t i=1;i<=m;++i)
            for(size_t i=m;i>0;--i)
            {
                if(!F(Q[i])) continue;
                std::cerr << "F=" << F << std::endl;
                if(qmatrix::fully_grown==F.active_state) break;
            }
            if(qmatrix::fully_grown!=F.active_state) throw imported::exception(fn,"ortho span RAVEn failure");


        }


        void nexus:: conserved_set(const xmlog &xml)
        {
            static const char * const fn = "conserved_set";
            YACK_XMLSUB(xml,fn);
            for(cluster *sharing=related.head;sharing;sharing=sharing->next)
            {
                conserved_set_(*sharing,xml);
            }
        }

        
    }
    
}

