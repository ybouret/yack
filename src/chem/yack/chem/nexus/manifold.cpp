#include "yack/chem/nexus.hpp"
#include "yack/counting/comb.hpp"
#include "yack/data/list/sort.hpp"
#include "yack/apex/kernel.hpp"
#include "yack/math/iota.hpp"
#include "yack/system/imported.hpp"
#include "yack/math/algebra/ortho-family.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/data/small/repo.hpp"
#include <iomanip>

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

        static inline size_t  active_coefficients(const readable<int> &arr) throw()
        {
            size_t count = 0;
            for(size_t i=arr.size();i>0;--i)
            {
                if( 0 != arr[i] ) ++count;
            }
            return count;
        }

        void nexus:: make_manifold(const xmlog &xml)
        {
            static const char * const fn = "make_manifold";
            YACK_XMLSUB(xml,fn);

            vector<equilibrium *> eqptr(N,as_capacity); //< inside this sharing
            addrbook              tribe; // to be populated by eqs
            sp_repo               cache; // from tribe
            small_repo<size_t>    plural; // nref>1
            small_repo<size_t>    lonely; // nref<=1
            const apq _0 = 0;
            const apq _1 = 1;

            //------------------------------------------------------------------
            //
            //
            //
            // outer loop: use a cluster of related equilibria
            //
            //
            //
            //------------------------------------------------------------------
            for(const cluster *sharing=related.head;sharing;sharing=sharing->next)
            {
                YACK_XMLSUB(xml, "sub_manifold");
                YACK_XMLOG(xml,*sharing);

                const size_t n = sharing->size;
                if(n<=1) {
                    YACK_XMLOG(xml, "-- standalone");
                    continue;
                }

                //------------------------------------------------------------------
                //
                //
                // collecting sharing equilibria with eqptr
                //
                //
                //------------------------------------------------------------------
                eqptr.free();
                for(const eq_node *en=sharing->head;en;en=en->next)
                {
                    const equilibrium &eq = **en;
                    eqptr.push_back( & coerce(eq) );
                }
                assert(eqptr.size()==n);



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
                        //
                        // create sub equilibria
                        //
                        //------------------------------------------------------
                        comb.extract(esub,eqptr);

                        //------------------------------------------------------
                        //
                        // extract all species, sorted by index
                        //
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
                        //
                        // extract sub-matrix with rank=k and m species
                        // and register species index with multiple refs.
                        //
                        //------------------------------------------------------
                        imatrix               nu(k,m);
                        {
                            plural.free();
                            lonely.free();
                            size_t j=1;
                            for(const sp_node *sn=cache->head;sn;sn=sn->next,++j)
                            {
                                size_t         nref = 0;
                                const species &sp   = **sn;
                                const size_t   sj   =  *sp;
                                for(size_t i=k;i>0;--i)
                                {
                                    const size_t         ei   = **esub[i];
                                    if( 0 != (nu[i][j] = Nu[ei][sj]) ) ++nref;
                                }
                                assert(nref>0);
                                if(nref>1)
                                {
                                    plural.push_back(j);
                                }
                                else
                                {
                                    lonely.push_back(j);
                                }
                            }
                            assert( apk::rank_of(nu) == k);
                        }


                        if(verbose) {
                            *xml << "-- [";
                            for(size_t i=1;i<=k;++i)
                                std::cerr << ' '  << esub[i]->name;
                            std::cerr << " ] / "  << cache.list << " => " << k << "x" << m << std::endl;
                            *xml << "   |_nu=" << nu << std::endl;
                            *xml << "   |_|plural| = " << std::setw(3) << plural->size << " : " << *plural <<  std::endl;
                            *xml << "   |_|lonely| = " << std::setw(3) << lonely->size << " : " << *lonely <<  std::endl;

                        }

                        //------------------------------------------------------
                        //
                        // build matrix of coefficients for each shared species
                        //
                        //------------------------------------------------------
                        const size_t p = plural->size; if(p<=0) continue; // maybe...
                        imatrix      mu(p,k);
                        {
                            size_t i=1;
                            for(const small_node<size_t> *pn=plural->head;pn;pn=pn->next,++i)
                            {
                                const size_t ii = **pn;
                                for(size_t j=k;j>0;--j)
                                    mu[i][j] = nu[j][ii];
                            }
                        }
                        const size_t r = apk::rank_of(mu);
                        const size_t l = lonely->size;
                        if(r+l<k)
                        {
                            throw imported::exception(fn,"invalid topology");
                        }

                        //------------------------------------------------------
                        //
                        // build auxiliary matrix of lonely species
                        //
                        //------------------------------------------------------
                        imatrix      lm;
                        if(l>0)
                        {
                            lm.make(l,k);
                            size_t i=1;
                            for(const small_node<size_t> *pn=lonely->head;pn;pn=pn->next,++i)
                            {
                                const size_t ii = **pn;
                                for(size_t j=k;j>0;--j)
                                    lm[i][j] = nu[j][ii];
                            }
                        }
                        YACK_XMLOG(xml,"   |_mu=" << mu << ", rank = " << r << " / " << k);
                        YACK_XMLOG(xml,"   |_lm=" << lm);



                    }
                    while(comb.next());
                }


            }




        }

    }

}

