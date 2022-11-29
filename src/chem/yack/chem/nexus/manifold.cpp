#include "yack/chem/nexus.hpp"
#include "yack/counting/comb.hpp"
#include "yack/data/list/sort.hpp"
#include "yack/apex/kernel.hpp"
#include "yack/math/iota.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    using namespace math;

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
            addrbook   tribe;
            sp_repo    cache;
            const apq _0 = 0;
            const apq _1 = 1;

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
                    combination           comb(n,k);  // combination
                    vector<equilibrium *> esub(k);    // sub-equilibria
                    vector<int>           coef(k);    // shared coefficient
                    imatrix               topo(k,M);  // local topology
                    matrix<apq>           Q(k,k);
                    do
                    {
                        cache.free();

                        //------------------------------------------------------
                        // create sub equilibria
                        //------------------------------------------------------
                        comb.extract(esub,eptr);

                        //------------------------------------------------------
                        // extract all species
                        //------------------------------------------------------
                        tribe.free();
                        for(size_t i=k;i>0;--i)
                            esub[i]->update(tribe);

                        for(addrbook::const_iterator it=tribe.begin();it!=tribe.end();++it)
                        {
                            cache.push_back( *static_cast<const species *>( *it ) );
                        }
                        merge_list_of<sp_node>::sort(cache.list,sp_node_compare);

                        //------------------------------------------------------
                        // build sub matrix
                        //------------------------------------------------------
                        topo.ld(0);
                        for(size_t i=k;i>0;--i)
                        {
                            const size_t         ei = **esub[i];
                            const readable<int> &nu = Nu[ei];
                            writable<int>       &to = topo[i];
                            for(const sp_node *sn=cache->head;sn;sn=sn->next)
                            {
                                const size_t j=***sn;
                                to[j] = nu[j];
                            }
                        }


                        if(verbose) {
                            *xml << "-- [";
                            for(size_t i=1;i<=k;++i)
                                std::cerr << ' '  << esub[i]->name;
                            std::cerr << " ] / "  << cache.list << std::endl;
                            *xml << "   |_topo=" << topo << std::endl;
                        }

                        //------------------------------------------------------
                        // extract shared coeff
                        //------------------------------------------------------
                        for(size_t j=M;j>0;--j)
                        {
                            size_t l = 0;
                            for(size_t i=k;i>0;--i)
                            {
                                if( 0 != (coef[i] = topo[i][j]) )
                                    ++l;
                            }
                            if(l>=k)
                            {
                                Q.ld(_0);
                                iota::load(Q[1],coef);
                                for(size_t i=k;i>1;--i)
                                {
                                    Q[i][i] = _1;
                                }
                                //std::cerr << "-- try " << Q << std::endl;
                                if(!apk::gs_ortho(Q))
                                {
                                    throw imported::exception(clid,"%s bad matrix",fn);
                                }
                                YACK_XMLOG(xml,"   |_ortho=" << Q);
                                for(size_t i=2;i<=k;++i)
                                {
                                    //*xml << "   |_ortho=" << Q[i] << std::endl;
                                }

                            }
                        }


                    }
                    while(comb.next());
                }

            }




        }

    }

}

