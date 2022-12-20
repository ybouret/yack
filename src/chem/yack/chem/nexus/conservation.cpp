
#include "yack/chem/nexus.hpp"
#include "yack/math/iota.hpp"
#include "yack/raven/qbranch.hpp"
#include "yack/math/algebra/ortho-family.hpp"
#include "yack/apex/kernel.hpp"

#include <iomanip>

namespace yack
{
    using namespace math;
    
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

            matrix<int> Q(m,m);
            if(!ortho_family::construct(Q,nu)) throw imported::exception(fn,"singular sub-system");
            std::cerr << "Q=" << Q << std::endl;
            

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

