#include "yack/chem/nexus.hpp"
#include "yack/counting/comb.hpp"
#include "yack/counting/perm.hpp"
#include "yack/data/list/sort.hpp"
#include "yack/apex/kernel.hpp"
#include "yack/math/iota.hpp"
#include "yack/system/imported.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/sequence/cxx-series.hpp"
#include "yack/data/small/repo.hpp"
#include "yack/sequence/roll.hpp"
#include "yack/math/algebra/ortho-family.hpp"
#include "yack/sequence/bunch.hpp"

#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {


        
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


        size_t make_clan(sp_list                       &clan,
                         const readable<equilibrium *> &esub)
        {
            const size_t k = esub.size();

            {
                //--------------------------------------------------------------
                // populate tribe of species
                //--------------------------------------------------------------
                addrbook tribe;
                for(size_t i=k;i>0;--i) esub[i]->update(tribe);

                //--------------------------------------------------------------
                // populate cache of species
                //--------------------------------------------------------------
                for(addrbook::const_iterator it=tribe.begin();it!=tribe.end();++it)
                    clan << static_cast<const species *>( *it );
            }

            //------------------------------------------------------------------
            // sort cache by increasing species index
            //------------------------------------------------------------------
            merge_list_of<sp_node>::sort(clan,sp_node_compare);
            const size_t m = clan.size;
            if(m<k) throw imported::exception("nexus::make_clan","not enough species!!");
            return m;
        }

        void select_rows(imatrix       &w,
                           const imatrix &mu)
        {
            const size_t   m = mu.rows;
            vector<size_t> jrow(m,as_capacity);

            // find non proportional rows
            for(size_t j=1;j<=m;++j)
            {
                const readable<int> &curr = mu[j];
                bool                 isOk = true;
                for(size_t k=jrow.size();k>0;--k) {
                    if( apk::are_prop(mu[ jrow[k] ],curr,NULL) )
                    {
                        isOk = false;
                        break;
                    }
                }
                if(isOk)
                    jrow << j;
            }

            // build sub-matrix
            const size_t dims = jrow.size();
            if(dims>0)
            {
                w.make(dims,mu.cols);
                for(size_t k=dims;k>0;--k)
                    iota::load(w[k],mu[ jrow[k] ]);
            }

        }

        static inline size_t count_valid(const readable<int> &coef) throw()
        {
            size_t count = 0;
            for(size_t i=coef.size();i>0;--i)
            {
                if( 0 != coef[i] ) ++count;
            }
            return count;
        }


        static inline bool whole_valid(const readable<int> &arr) throw()
        {
            for(size_t i=arr.size();i>0;--i)
            {
                if( 0 == arr[i] ) return false;
            }
            return true;
        }


        static inline void q2i(writable<int>       &cof,
                               const readable<apq> &q)
        {
            assert(cof.size()==q.size());
            for(size_t i=cof.size();i>0;--i)
            {
                assert(1==q[i].den);
                cof[i] = q[i].num.cast_to<int>();
            }
        }

        static inline void fill_topo(imatrix                      &nu,
                                     const imatrix                &Nu,
                                     const readable<equilibrium*> &esub,
                                     const sp_list                &clan) throw()
        {
            for(size_t i=esub.size();i>0;--i)
            {
                const size_t ei = **esub[i];
                size_t       j  = 1;
                for(const sp_node *sp=clan.head;sp;sp=sp->next,++j)
                {
                    const size_t sj = ***sp;
                    nu[i][j] = Nu[ei][sj];
                }
            }
        }

        void process(bunch<int>   &coeff, const imatrix &mu)
        {
            const size_t m = mu.rows;
            const size_t n = mu.cols;
            assert(coeff.width==n);


            std::cerr << "processing work=" << mu << std::endl;
            vector<size_t> pad(m-1);
            matrix<apq>    sub(n,n);
            cxx_array<int> cof(n);

            for(size_t j=1;j<=m;++j)
            {
                if(count_valid(mu[j]) < 2 ) continue;

                pad.free();
                for(size_t jj=1;jj<j;++jj)    pad << jj;
                for(size_t jj=j+1;jj<=m;++jj) pad << jj;
                assert(m-1==pad.size());
                std::cerr << "  -> " << mu[j] << "  -> amongst jrow=" << pad << std::endl;

                //combination go(m-1,n-1);
                permutation go(m-1);
                size_t      ok=0;
                do
                {
                    iota::load(sub[1],mu[j]);
                    for(size_t j=2,i=1;j<=n;++j,++i)
                    {
                        iota::load(sub[j],mu[ pad[ go[i] ] ]);
                    }
                    std::cerr << "\tgo=" << go << " sub=" << sub;

                    if(!apk::gs_ortho(sub)) {
                        std:: cerr << " singular" << std::endl;
                        continue;
                    }

                    std::cerr << "\t  -> " << sub << std::endl;
                    for(size_t j=2;j<=n;++j)
                    {
                        ++ok;
                        q2i(cof,sub[j]);
                        std::cerr << "    -> try " << ok << " : " << cof;
                        if(whole_valid(cof))
                        {
                            std::cerr << " [*]" << std::endl;
                            coeff.ensure(cof);
                        }
                        else
                        {
                            std::cerr << " [-]" << std::endl;
                        }
                    }
                }
                while(go.next());
                std::cerr << std::endl;
            }
            std::cerr << *coeff << std::endl;
        }

        void nexus:: make_manifold_(cluster &source, const xmlog &xml)
        {
            static const char * const fn = "sub_manifold";
            YACK_XMLSUB(xml, "sub_manifold");
            YACK_XMLOG(xml,source);

            //------------------------------------------------------------------
            //
            // get equilibria within this cluster
            //
            //------------------------------------------------------------------
            const size_t n = source.size;
            if(n<=1) {
                YACK_XMLOG(xml, "<standalone>");
                return;
            }

            cxx_array<equilibrium *> edb(n);
            {
                size_t i=1;
                for(const eq_node *en=source.head;en;en=en->next,++i)
                    edb[i] = &coerce(**en);
            }

            //------------------------------------------------------------------
            //
            //
            // loop over all k-uplets from 2 to n
            //
            //
            //------------------------------------------------------------------
            for(size_t k_=2;k_<=n;++k_)
            {
                const size_t             k = k_;
                combination              comb(n,k);  // possible combination
                cxx_array<equilibrium *> esub(k);    // of selected equilibria
                do
                {
                    //----------------------------------------------------------
                    //
                    // create local equilibria subset
                    //
                    //----------------------------------------------------------
                    comb.designate(esub,edb);

                    //----------------------------------------------------------
                    //
                    // create local species from equilibria
                    //
                    //----------------------------------------------------------
                    sp_list      clan;
                    const size_t m = make_clan(clan,esub);

                    //----------------------------------------------------------
                    //
                    // extract local topology
                    //
                    //----------------------------------------------------------
                    const imatrix nu(k,m); fill_topo( coerce(nu), Nu, esub, clan);
                    const imatrix mu(nu,transposed);


                    *xml << "-- [";
                    for(size_t i=1;i<=k;++i)
                        std::cerr << ' '  << esub[i]->name;
                    std::cerr << " ] / " << k << std::endl;
                    std::cerr << "\tnu=" << nu << std::endl;
                    std::cerr << "\tmu=" << mu << std::endl;

                    if( k != apk::rank_of(mu) ) throw imported::exception(fn,"invalid sub-system topology!");

                    //----------------------------------------------------------
                    //
                    // compress rows
                    //
                    //----------------------------------------------------------
                    imatrix     w;
                    select_rows(w,mu);
                    std::cerr << "\tw =" << w << std::endl;

                    bunch<int> coeff(k);
                    process(coeff,w);
                    



                } while(comb.next());
            }




        }


        void nexus:: make_manifold(const xmlog &xml)
        {
            static const char * const fn = "make_manifold";
            YACK_XMLSUB(xml,fn);

            for(cluster *sharing=related.head;sharing;sharing=sharing->next)
                make_manifold_(*sharing,xml);

        }

    }

}

