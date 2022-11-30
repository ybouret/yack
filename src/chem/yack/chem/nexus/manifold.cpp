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

        
        static inline bool non_degenerated(const readable<int> &arr) throw()
        {
            for(size_t i=arr.size();i>0;--i)
            {
                if( 0 == arr[i] ) return false;
            }
            return true;
        }

        typedef cxx_array<int> stoi_coef;

        class stoi_node : public object, public stoi_coef
        {
        public:
            explicit stoi_node(const readable<int> &arr) :
            object(),
            stoi_coef( arr.size() ),
            next(0)
            {
                iota::load(*this,arr);
            }

            stoi_node *next;
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(stoi_node);
        };

        class stoi_repo : public cxx_pool_of<stoi_node>
        {
        public:
            explicit stoi_repo() throw() : cxx_pool_of<stoi_node>() {}
            virtual ~stoi_repo() throw() {}

            void ensure(const readable<int> &lhs)
            {
                for(const stoi_node *node=head;node;node=node->next)
                {
                    const readable<int> &rhs = *node;
                    assert(rhs.size()==lhs.size());
                    bool same = true;
                    for(size_t i=lhs.size();i>0;--i)
                    {
                        if(lhs[i]!=rhs[i]) {
                            same = false;
                            break;
                        }
                    }
                    if(same) return;
                }

                // success
                store( new stoi_node(lhs) );

            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(stoi_repo);
        };



        static inline
        string makeName(const readable<int> &cof,
                        const equilibria    &eqs)
        {
            string res;
            bool   first = true;
            for(const enode *en=eqs.head();en;en=en->next)
            {
                const equilibrium &eq = ***en;
                const size_t       ei = *eq;
                const int          cf = cof[ei];
                switch(__sign::of(cf))
                {
                    case __zero__: continue;
                    case positive:
                        if(first)
                        {
                            first = false;
                        }
                        else
                            res += '+';
                        if(cf>1) res += vformat("%d*",cf);
                        break;

                    case negative:
                        if(first)
                        {
                            first = false;
                        }
                        if(cf < -1)
                            res += vformat("%d*",cf);
                        else
                            res += '-';
                        break;
                }
                res += eq.name;
            }
            return res;
        }


        void nexus:: make_manifold(const xmlog &xml)
        {
            typedef small_node<size_t> qnode;
            static const char * const fn = "make_manifold";
            YACK_XMLSUB(xml,fn);

            vector<equilibrium *> eqptr(N,as_capacity); // inside this sharing
            vector<size_t>        anx(M,as_capacity);   // to form ortho matrix
            addrbook              tribe;                // to be populated by eqs
            sp_repo               cache;                // from tribe
            small_repo<size_t>    plural;               // nref>1
            sp_repo               fading;               // matching plural
            cxx_array<int>        gmix(N);
            stoi_repo             extra;
            cxx_array<int>        gcoef(M);

            //------------------------------------------------------------------
            //
            //
            //
            // outer loop: use a cluster of related equilibria
            //
            //
            //
            //------------------------------------------------------------------
            for(cluster *sharing=related.head;sharing;sharing=sharing->next)
            {
                YACK_XMLSUB(xml, "sub_manifold");
                YACK_XMLOG(xml,*sharing);

                extra.release();
                const size_t n = sharing->size;
                if(n<=1) {
                    YACK_XMLOG(xml, "-- standalone");
                    continue;
                }




                //--------------------------------------------------------------
                //
                //
                // collecting sharing equilibria with eqptr
                //
                //
                //--------------------------------------------------------------
                eqptr.free();
                for(const eq_node *en=sharing->head;en;en=en->next)
                {
                    const equilibrium &eq = **en;
                    eqptr.push_back( & coerce(eq) );
                }
                assert(eqptr.size()==n);



                //--------------------------------------------------------------
                //
                //
                //
                // inner loop: try to associate elements of this cluster
                //
                //
                //
                //--------------------------------------------------------------
                for(size_t  k=2;k<=n;++k)
                {
                    combination           comb(n,k);  // combination
                    vector<equilibrium *> esub(k);    // sub-equilibria
                    imatrix               sub(k,k);   // sub matrix of coeffs
                    matrix<apq>           mgs(k,k);   // to compute ortho space
                    cxx_array<int>        icf(k);     // integer coefficient

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
                            fading.free();
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
                                    fading.push_back(sp);
                                }

                            }
                            assert( apk::rank_of(nu) == k);
                        }

                        imatrix mu(nu,transposed);


                        if(verbose) {
                            *xml << "-- [";
                            for(size_t i=1;i<=k;++i)
                                std::cerr << ' '  << esub[i]->name;
                            std::cerr << " ] / "  << cache.list << " => " << k << "x" << m << std::endl;
                            *xml << "   |_nu=" << nu << std::endl;
                            *xml << "   |_mu=" << mu << std::endl;
                            //*xml << "   |_|plural| = " << std::setw(3) << plural->size << " : " << *plural <<  std::endl;
                            *xml << "   |_|fading| = " << std::setw(3) << fading->size << " : " << *fading <<  std::endl;

                        }

                        const size_t p = plural->size; if(p<=0) continue; // maybe...

                        //------------------------------------------------------
                        //
                        // now find all ways to make a full zero concentration
                        //
                        //------------------------------------------------------
                        stoi_repo        mix;
                        sp_node         *sn = fading->head;
                        for(const qnode *qn = plural->head;qn;qn=qn->next,sn=sn->next)
                        {
                            //--------------------------------------------------
                            // initialize first row and annex indices
                            //--------------------------------------------------
                            sub.ld(0);
                            anx.free();
                            const size_t i = **qn;
                            for(size_t j=1;j<i;++j)    anx << j;
                            for(size_t j=i+1;j<=m;++j) anx << j;
                            assert(m-1==anx.size());
                            iota::load(sub[1],mu[i]);
                            //YACK_XMLOG(xml,"   |_fading [" << **sn << "] using " << sub[1] << " / anx=" << anx);

                            //--------------------------------------------------
                            // exposing chosen row to rolling others
                            //--------------------------------------------------
                            size_t cycle=1;
                        CYCLE:
                            //--------------------------------------------------
                            // construct sub with current annex indices
                            //--------------------------------------------------
                            for(size_t i=2,ia=1;i<=k;++i,++ia)
                            {
                                iota::load(sub[i],mu[anx[ia]]);
                            }
                            mgs.assign(sub);
                            if( apk::gs_ortho(mgs) )
                            {
                                // extracting non degenerate solutions
                                for(size_t i=2;i<=k;++i)
                                {
                                    const readable<apq> &q = mgs[i];
                                    for(size_t j=k;j>0;--j)
                                    {
                                        icf[j] = q[j].num.cast_to<int>();
                                    }
                                    assert( iota::dot<int>::of(sub[1],icf) == 0);
                                    if(non_degenerated(icf))
                                    {
                                        mix.ensure(icf);
                                    }
                                }

                            }


                            //--------------------------------------------------
                            // rotate indices
                            //--------------------------------------------------
                            if(++cycle<m)
                            {
                                const size_t a1 = anx[1];
                                for(size_t ip=1,i=2;i<m;++i,++ip)
                                {
                                    cswap(anx[ip],anx[i]);
                                }
                                anx.back() = a1;
                                goto CYCLE;
                            }
                        }

                        //------------------------------------------------------
                        //
                        // collecting all mixes
                        //
                        //------------------------------------------------------
                        for(const stoi_node *node=mix.head;node;node=node->next)
                        {
                            const readable<int> &cf = *node;
                            gmix.ld(0);
                            for(size_t i=1;i<=k;++i)
                            {
                                const int          f  = cf[i];
                                const equilibrium &eq = *esub[i];
                                gmix[*eq] = f;
                            }
                            extra.ensure(gmix);
                        }

                    }
                    while(comb.next());
                }


                //--------------------------------------------------------------
                //
                //
                // creating more equilibria within this cluster
                //
                //
                //--------------------------------------------------------------
                for(const stoi_node *node=extra.head;node;node=node->next)
                {
                    const readable<int> &weight = *node;
                    gcoef.ld(0);
                    for(const enode *en=singles.head();en;en=en->next)
                    {
                        const equilibrium &eq = ***en;
                        const size_t       ei = *eq;
                        const int          ew = weight[ei];
                        if(!ew) continue;;
                        for(const cnode *cn=eq.head();cn;cn=cn->next)
                        {
                            const size_t j = *****cn;
                            gcoef[j] += ew * Nu[ei][j];
                        }
                    }


                    const string name = makeName(weight,singles);
                    std::cerr << weight << " => '" << name << "'";

                    components mock;
                    for(size_t j=1;j<=M;++j)
                    {
                        const int f = gcoef[j];
                        if(f) mock( worklib[j], f);
                    }

                    std::cerr << " -> " << mock << std::endl;
                    assert(mock.neutral());
                }


            }




        }

    }

}

