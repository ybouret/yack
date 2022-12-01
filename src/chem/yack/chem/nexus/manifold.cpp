#include "yack/chem/nexus.hpp"
#include "yack/counting/comb.hpp"
#include "yack/data/list/sort.hpp"
#include "yack/apex/kernel.hpp"
#include "yack/math/iota.hpp"
#include "yack/system/imported.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/data/small/repo.hpp"
#include "yack/sequence/roll.hpp"
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {

        
        static inline bool non_degenerated(const readable<int> &arr) throw()
        {
            for(size_t i=arr.size();i>0;--i)
            {
                if( 0 == arr[i] ) return false;
            }
            return true;
        }

        namespace
        {
            //! array of coefficients
            typedef cxx_array<int> stoi_coef;
            
            //! used to store coefficients
            class stoi_node : public object, public stoi_coef
            {
            public:
                inline stoi_node(const readable<int> &arr) :
                object(), stoi_coef( arr.size() ), next(0) {
                    iota::load(*this,arr);
                }

                inline ~stoi_node() throw() {}

                stoi_node *next;
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(stoi_node);
            };

            typedef cxx_pool_of<stoi_node> stoi_assembly;

            //! used to store assembly of coefficients
            class stoi_repo : public stoi_assembly
            {
            public:
                explicit stoi_repo() throw() : stoi_assembly() {}
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
            
        }
        
        namespace
        {
            class mixed_equilibrium : public equilibrium
            {
            public:
                virtual ~mixed_equilibrium() throw()
                {
                }
                
                const readable<double> &Ktab;
                rmulops                &xmul;
                const cxx_array<int>    coef;
                
                explicit mixed_equilibrium(const string           &uid,
                                           const size_t            idx,
                                           const readable<double> &myK,
                                           rmulops                &ops,
                                           const readable<int>    &arr) :
                equilibrium(uid,idx),
                Ktab(myK),
                xmul(ops),
                coef(arr)
                {
                    
                }
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(mixed_equilibrium);
                virtual double getK(double) const
                {
                    xmul.ld1();
                    for(size_t j=coef.size();j>0;--j)
                    {
                        xmul.ipower(Ktab[j],coef[j]);
                    }
                    return xmul.query();
                }
            };
        }


        const equilibrium &nexus:: promote_mixed(const readable<int> &weight)
        {
            cxx_array<int> gcoef(M);
            for(const enode *en=singles.head();en;en=en->next)
            {
                const equilibrium &eq = ***en;
                const size_t       ei = *eq;
                const int          ew = weight[ei];
                if(!ew) continue;
                for(const cnode *cn=eq.head();cn;cn=cn->next)
                {
                    const size_t j = *****cn;
                    gcoef[j] += ew * Nu[ei][j];
                }
            }

            //----------------------------------------------------------
            // create a mixed equilibrium
            //----------------------------------------------------------
            equilibria  &target = coerce(lattice);
            const string name   = singles.make_name(weight);
            const size_t mxid   = target.size()+1;
            equilibrium &mxeq   = target.use( new mixed_equilibrium(name,mxid,K,xmul,weight) );
            for(size_t j=1;j<=M;++j)
            {
                const int f = gcoef[j];
                if(f) mxeq( worklib[j], f);
            }

            assert(mxeq.neutral());

            //----------------------------------------------------------
            // to register in this related group
            //----------------------------------------------------------
            return mxeq;

        }


        void nexus:: make_manifold(const xmlog &xml)
        {
            typedef small_node<size_t> qnode;
            static const char * const fn = "make_manifold";
            YACK_XMLSUB(xml,fn);

            vector<size_t>        pad(M,as_capacity);   // to form ortho matrix
            small_repo<size_t>    plural;               // nref>1
            sp_repo               fading;               // matching plural
            cxx_array<int>        gmix(N);


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

                const size_t n = sharing->size;
                if(n<=1) {
                    YACK_XMLOG(xml, "-- standalone");
                    continue;
                }

                stoi_repo             extra;                // unique coefficients
                vector<equilibrium *> eptr(n,as_capacity);  // from sharing size


                //--------------------------------------------------------------
                //
                //
                // collecting sharing equilibria within eptr
                //
                //
                //--------------------------------------------------------------
                for(const eq_node *en=sharing->head;en;en=en->next)
                {
                    const equilibrium &eq = **en;
                    eptr.push_back( & coerce(eq) );
                }
                assert(eptr.size()==n);



                //--------------------------------------------------------------
                //
                //
                //
                // inner loop: try to associate elements of this cluster
                // by k-uplets
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
                        // create sub-system of k equilibria
                        //
                        //------------------------------------------------------
                        comb.extract(esub,eptr);

                        //------------------------------------------------------
                        //
                        // extract all species, sorted by index
                        //
                        //------------------------------------------------------
                        sp_list cache;
                        {
                            // populate tribe of unique species
                            addrbook tribe;
                            for(size_t i=k;i>0;--i) esub[i]->update(tribe);

                            // populate cache of species
                            for(addrbook::const_iterator it=tribe.begin();it!=tribe.end();++it)
                                cache << static_cast<const species *>( *it );

                            // sort cache by increasing species index
                            merge_list_of<sp_node>::sort(cache,sp_node_compare);
                        }
                        const size_t m = cache.size; assert(m>0);
                        

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
                            for(const sp_node *sn=cache.head;sn;sn=sn->next,++j)
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
                            std::cerr << " ] / "  << cache << " => " << k << "x" << m << std::endl;
                            *xml << "   |_nu=" << nu << std::endl;
                            *xml << "   |_mu=" << mu << std::endl;
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
                            // initialize first row and padding indices
                            //--------------------------------------------------
                            sub.ld(0);
                            pad.free();
                            const size_t i = **qn;
                            for(size_t j=1;j<i;++j)    pad << j;
                            for(size_t j=i+1;j<=m;++j) pad << j;
                            assert(m-1==pad.size());
                            iota::load(sub[1],mu[i]);

                            //--------------------------------------------------
                            // exposing chosen row to rolling others
                            //--------------------------------------------------
                            size_t rotations=1;
                        ROTATION:
                            //--------------------------------------------------
                            // construct sub with current annex indices
                            //--------------------------------------------------
                            for(size_t i=2,ia=1;i<=k;++i,++ia)
                            {
                                iota::load(sub[i],mu[pad[ia]]);
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
                            if(++rotations<m)
                            {
                                rolling::down(pad);
                                goto ROTATION;
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
                    *sharing << &promote_mixed(*node);


                YACK_XMLOG(xml, "-- |cluster| = " << n << " -> " << sharing->size);
                exit(0);
            }


            if(verbose)
            {
                std::cerr << lattice << std::endl;
            }

        }

    }

}

