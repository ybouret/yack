#include "yack/chem/nexus.hpp"
#include "yack/counting/comb.hpp"
#include "yack/data/list/sort.hpp"

namespace yack
{
    namespace chemical
    {

        static inline int sp_node_compare(const sp_node *lhs, const sp_node *rhs) throw()
        {
            return comparison::increasing( ***lhs, ***rhs );
        }

        void nexus:: make_manifold(const xmlog &xml)
        {
            static const char * const fn = "make_manifold";
            YACK_XMLSUB(xml,fn);

            //------------------------------------------------------------------
            //
            // outer loop: use a cluster of related equilibria
            //
            //------------------------------------------------------------------
            addrbook tribe;
            sp_repo  cache;

            for(const cluster *sharing=related.head;sharing;sharing=sharing->next)
            {
                YACK_XMLSUB(xml, "sub_manifold");
                YACK_XMLOG(xml,*sharing);

                const size_t n = sharing->size;
                if(n<=1) continue;
                vector<equilibrium *> eptr(n,as_capacity);

                for(const eq_node *en=sharing->head;en;en=en->next)
                {
                    const equilibrium &eq = **en;
                    eptr.push_back( & coerce(eq) );
                }

                for(size_t k=2;k<=n;++k)
                {
                    combination           comb(n,k);
                    vector<equilibrium *> esub(k);    // sub-equilibria
                    vector<int>           coef(k);    // coefficient for one species

                    do
                    {
                        tribe.free();
                        cache.free();
                        comb.extract(esub,eptr);
                        for(size_t i=k;i>0;--i)
                        {
                            esub[i]->update(tribe);
                        }

                        for(addrbook::const_iterator it=tribe.begin();it!=tribe.end();++it)
                        {
                            cache.push_back( *static_cast<const species *>( *it ) );
                        }
                        merge_list_of<sp_node>::sort(cache.list,sp_node_compare);

                        if(verbose) {
                            *xml << "-- [";
                            for(size_t i=1;i<=k;++i)
                                std::cerr << ' '  << esub[i]->name;
                            std::cerr << " ] / "  << cache.list << std::endl;
                        }

                        for(const anode *an=working.head;an;an=an->next)
                        {
                            const species &s = **an;
                            const size_t   j = *s;
                            size_t         l = 0;
                            for(size_t i=k;i>0;--i)
                            {
                                const size_t ei = **esub[i];
                                if( 0 != (coef[i] = Nu[ei][j]) ) ++l;
                            }
                            //std::cerr << "\t@" << s.name << " : " << coef << std::endl;
                        }
                    }
                    while(comb.next());
                }

            }




        }

    }

}

