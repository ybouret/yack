#include "yack/chem/nexus.hpp"
#include "yack/counting/comb.hpp"
#include "yack/data/list/sort.hpp"
#include "yack/apex/kernel.hpp"
#include "yack/math/iota.hpp"
#include "yack/system/imported.hpp"
#include "yack/math/algebra/ortho-family.hpp"
#include "yack/sequence/cxx-array.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {

        static inline int sp_node_compare(const sp_node *lhs, const sp_node *rhs) throw()
        {
            return comparison::increasing( ***lhs, ***rhs );
        }


        static inline bool have_common_index(const readable<size_t> &lhs,
                                             const readable<size_t> &rhs) throw()
        {
            for(size_t i=lhs.size();i>0;--i)
            {
                const size_t L = lhs[i];
                for(size_t j=rhs.size();j>0;--j)
                {
                    if(L==rhs[j]) return true;
                }
            }
            return false;
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
            addrbook   tribe; // to be populated by eqs
            sp_repo    cache; // from tribe
            const apq _0 = 0;
            const apq _1 = 1;

            for(const cluster *sharing=related.head;sharing;sharing=sharing->next)
            {
                YACK_XMLSUB(xml, "sub_manifold");
                YACK_XMLOG(xml,*sharing);

                const size_t n = sharing->size;
                if(n<=1)
                {
                    continue;
                }

                vector<equilibrium *> eptr(n,as_capacity); //!< inside this sharing
                vector<int>           stoi(M);             //!< resulting

                for(const eq_node *en=sharing->head;en;en=en->next)
                {
                    const equilibrium &eq = **en;
                    eptr.push_back( & coerce(eq) );
                }




                for(size_t k=2;k<=n;++k)
                {
                    combination           comb(n,k);  // combination
                    vector<equilibrium *> esub(k);    // sub-equilibria
                    cxx_array<int>        coef(k);    // shared coefficient
                    imatrix               sub(k,k);   // sub matrix of coeffs
                    matrix<apq>           mgs(k,k);   //

                    do
                    {

                        //------------------------------------------------------
                        // create sub equilibria
                        //------------------------------------------------------
                        comb.extract(esub,eptr);

                        //------------------------------------------------------
                        // extract all species, sorted by index
                        //------------------------------------------------------
                        tribe.free();
                        cache.free();
                        for(size_t i=k;i>0;--i) esub[i]->update(tribe);
                        for(addrbook::const_iterator it=tribe.begin();it!=tribe.end();++it)
                        {
                            cache.push_back( *static_cast<const species *>( *it ) );
                        }
                        merge_list_of<sp_node>::sort(cache.list,sp_node_compare);

                        const size_t m = cache->size; assert(m>0);

                        //------------------------------------------------------
                        // extract sub-matrix with rank=k and m species
                        //------------------------------------------------------
                        imatrix             nu(k,m);
                        imatrix             mu(m,k);
                        cxx_array<unsigned> nc(m);

                        for(size_t i=k;i>0;--i)
                        {
                            const size_t         ei   = **esub[i];
                            const readable<int> &topo = Nu[ei];
                            size_t               j    = 1;
                            for(const sp_node *sn=cache->head;sn;sn=sn->next,++j)
                            {
                                mu[j][i] = nu[i][j] = topo[***sn];
                            }
                        }
                        assert( apk::gj_rank_of(nu) == k);
                        assert( apk::gj_rank_of(mu) == k);
                        for(size_t j=m;j>0;--j) {
                            unsigned sum = 0;
                            for(size_t i=k;i>0;--i)
                            {
                                if(mu[j][i]!=0) ++sum;
                            }
                            nc[j] = sum;
                        }


                        if(verbose) {
                            *xml << "-- [";
                            for(size_t i=1;i<=k;++i)
                                std::cerr << ' '  << esub[i]->name;
                            std::cerr << " ] / "  << cache.list << std::endl;
                            *xml << "   |_nu=" << nu << std::endl;
                            *xml << "   |_mu=" << mu << std::endl;
                            *xml << "   |_nc=" << nc << std::endl;
                        }

#if 0
                        //------------------------------------------------------
                        // try to remove species [1:k-1] species
                        //------------------------------------------------------
                        for(size_t d=1;d<k;++d)
                        {
                            const size_t kmd = k-d;
                            const size_t num = kmd+1;;
                            combination  mix(m,d);
                            YACK_XMLOG(xml, "-- looking for order " << d << " vectors with |coef|=" << num << " amongst " << m);
                            do
                            {
                                sub.ld(0);
                                {
                                    bool success = true;
                                    for(size_t i=d;i>0;--i)
                                    {
                                        const size_t ii = mix[i];
                                        if(num!=nc[ii])
                                        {
                                            success = false;
                                            break;
                                        }
                                        iota::load(sub[i],mu[ii]);
                                    }
                                    if(!success) {
                                        continue;
                                    }
                                }
                                std::cerr << "=> " << sub << std::endl;
                                std::cerr << "=> filling with " << k-d << " extra vectors" << std::endl;
                                // complete matrix to full rank
                                {
                                    bool success = false;
                                    combination jam(m,kmd); // choosing amongst rows of mu
                                    do
                                    {
                                        if(have_common_index(mix,jam))
                                            continue;

                                        for(size_t i=d+1,j=1;i<=k;++i,++j)
                                        {
                                            iota::load(sub[i],mu[ jam[j] ]);
                                        }
                                        const size_t rank = apk::gj_rank_of(sub);
                                        if(rank>=k)
                                        {
                                            mgs.assign(sub);
                                            if(!apk::gs_ortho(mgs)) throw exception("bad gram-schmidt");
                                            std::cerr << "\t[ok] => " << mgs << std::endl;
                                            success = true;
                                            break;
                                        }
                                    } while( jam.next() );
                                    if(!success)
                                    {
                                        std::cerr << "BAD?" << std::endl;
                                        exit(0);
                                    }
                                }


                            } while( mix.next() );
                        }
#endif
                        


                    }
                    while(comb.next());
                }

                //std::cerr << stcof << std::endl;

            }




        }

    }

}

