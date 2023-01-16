#include "yack/chem/reactor/cluster.hpp"
#include "yack/system/imported.hpp"
#include "yack/apex/alga.hpp"
#include "yack/raven/qbranch.hpp"
#include "yack/raven/qselect.hpp"
#include "yack/sequence/bunch.hpp"
#include "yack/associative/lexicon.hpp"
#include "yack/data/dinky/solo-repo.hpp"

namespace yack
{
    using namespace raven;

    namespace chemical
    {
        namespace {

            //------------------------------------------------------------------
            //
            //! select more than two coefficients
            //
            //------------------------------------------------------------------
            template <typename T> static inline
            bool keep_more_than_two(const readable<T> &cf) throw()
            {
                return qselect::count_valid(cf) >= 2;
            }

            //------------------------------------------------------------------
            //
            //! local collector
            //
            //------------------------------------------------------------------
            class collector : public bunch<int>
            {
            public:
                //--------------------------------------------------------------
                //
                // C++
                //
                //--------------------------------------------------------------
                explicit collector(const size_t w) :
                bunch<int>(w)
                {}

                virtual ~collector() throw() {}

                //--------------------------------------------------------------
                //
                // callback for RAVEn
                //
                //--------------------------------------------------------------
                void operator()(const qvector &cf) {
                    if( qselect::count_valid(  cf.cast_to(work) ) >= 2 )
                    {
                        const readable<int> &native = work;
                        ensure(native);
                    }
                }


                //--------------------------------------------------------------
                // sort by norm1 and lexicographic
                //--------------------------------------------------------------
                inline void organize() { sort_with(compare); }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(collector);
                static inline int norm1(const readable<int> &arr) throw()
                {
                    int sum = 0;
                    for(size_t i=arr.size();i>0;--i)
                    {
                        sum += absolute(arr[i]);
                    }
                    return sum;
                }

                static inline int compare(const readable<int> &lhs,
                                          const readable<int> &rhs) throw()
                {
                    const int l1 = norm1(lhs);
                    const int r1 = norm1(rhs);
                    if(l1<r1)
                    {
                        return -1;
                    }
                    else
                    {
                        if(r1<l1)
                        {
                            return 1;
                        }
                        else
                        {
                            return comparison::lexicographic(lhs,rhs);
                        }
                    }
                }
            };


            //------------------------------------------------------------------
            //
            //! global to local topology
            //
            //------------------------------------------------------------------
            static inline
            void global_to_local_topology(matrix<int>       &nu,
                                          const glist       &grp,
                                          const alist       &act,
                                          const matrix<int> &Nu) throw()
            {
                for(const gnode *en=grp->head;en;en=en->next)
                {
                    const equilibrium   &eq = en->host;
                    const size_t         eI = *eq;
                    const size_t         ei = **en;
                    const readable<int> &nI = Nu[eI];
                    writable<int>       &ni = nu[ei];
                    for(const anode *sn=act->head;sn;sn=sn->next)
                    {
                        const species &sp = sn->host;
                        const size_t   sJ = *sp;
                        const size_t   sj = **sn;
                        ni[sj] = nI[sJ];
                    }
                }
            }

            //------------------------------------------------------------------
            //
            //! build manifold from eqs/species selection
            //
            //------------------------------------------------------------------
            static inline
            void build_manifold(const xmlog       &xml,
                                collector         &cw,
                                const glist       &grp,
                                const alist       &act,
                                const matrix<int> &Nu)
            {
                {
                    const size_t n = grp->size; assert(n>0);  // equilibria
                    const size_t m = act->size; assert(m>=n); // species
                    matrix<int>  mu;                          // compressed transposed topoplogy

                    //----------------------------------------------------------
                    // compute local topology and its transposed
                    //----------------------------------------------------------
                    {
                        matrix<int>  nu(n,m);
                        global_to_local_topology(nu,grp,act,Nu);
                        YACK_XMLOG(xml, "nu=" << nu);
                        if(n!=alga::rank(nu))            throw imported::exception(cluster::clid,"invalid topology rank");
                        if(n!=qselect::compress(mu,nu))  throw imported::exception(cluster::clid,"invalid transposed rank");
                        YACK_XMLOG(xml, "mu=" << mu);
                    }

                    //----------------------------------------------------------
                    // use RAVEn
                    //----------------------------------------------------------
                    {
                        qbranch qgen;
                        qgen.batch(mu,n,keep_more_than_two<int>,cw);
                    }
                }

                //--------------------------------------------------------------
                // finalize
                //--------------------------------------------------------------
                cw.organize();

            }

            //------------------------------------------------------------------
            //
            //! populate roaming from a set of equilibria
            //
            //------------------------------------------------------------------
            static inline
            void populate(addrbook &roaming, const eq_gnode *node)
            {
                for(;node;node=node->next)
                {
                    const equilibrium &eq = (***node).host;
                    roaming.ensure( &eq );
                }
            }

            //------------------------------------------------------------------
            //
            //! populate balance from a set of equilibria
            //
            //------------------------------------------------------------------
            static inline
            void populate(eq_repo_ &balance, const eq_gnode *node)
            {
                for(;node;node=node->next)
                {
                    const equilibrium &eq = (***node).host;
                    balance << eq;
                }
            }

            //------------------------------------------------------------------
            //
            //! detect if a native (local) equilibrium is roaming
            //
            //------------------------------------------------------------------
            static inline
            bool has_roaming(const readable<int> &native,
                             const gnode         *node,
                             const addrbook      &roaming)
            {
                for(size_t i=native.size();i>0;--i,node=node->next)
                {
                    if( native[**node] && roaming.search( &(node->host)) ) return true;
                }
                return false;
            }

        }


        typedef lexicon<size_t> lexicon_type;

        //! store INDICES of not null coefficients
        static inline
        void store(lexicon_type  &sto, const readable<int> &nu)
        {
            for(size_t j=nu.size();j>0;--j)
            {
                const int coef = nu[j];
                if(coef) sto.ensure(j);
            }
        }

        //! store all not zero coefficients INDICES
        static inline
        void store(lexicon_type        &sto,
                   const readable<int> &weight,
                   const matrix<int>   &Nu)
        {
            sto.free();
            for(size_t i=weight.size();i>0;--i)
            {
                if( weight[i] ) store(sto,Nu[i]); // extrac indices
            }
        }



        void cluster:: make_manifold(const xmlog            &xml,
                                     const matrix<int>      &Nu,
                                     const readable<double> &K,
                                     const library          &lib,
                                     const equilibria       &eqs,
                                     equilibria             &all)
        {
            YACK_XMLSUB(xml,"cluster::make_manifold");
            const glist &grp = *group;

            //------------------------------------------------------------------
            //
            // check status
            //
            //------------------------------------------------------------------
            const size_t n = grp->size; assert(n>0);
            switch(n)
            {
                case  0: throw imported::exception(clid,"unexpected empty cluster!!");
                case  1: YACK_XMLOG(xml, "-- standalone <" << (*group)->head->host.name << ">" ); return;
                default: break;
            }

            //------------------------------------------------------------------
            //
            // register first degree equilibria (primary layout)
            //
            //------------------------------------------------------------------
            for(const glist::node_type *node=grp->head;node;node=node->next)
                coerce( *(cross->front()) ) << node->host;


            //------------------------------------------------------------------
            //
            // build manifold by RAVEn on local topology
            //
            //------------------------------------------------------------------
            collector    cw(n);
            build_manifold(xml,cw,grp,*alive,Nu);

            
            //------------------------------------------------------------------
            //
            // creating cross/mixed equilibria
            //
            //------------------------------------------------------------------
            {
                ledger        &layout = coerce( *cross );              // register layouts
                cxx_array<int> weight(Nu.rows);                        // global weight
                cxx_array<int> stoich(Nu.cols);                        // global stoich
                addrbook       roaming;                                // roaming equilibria
                eq_repo       &balance = coerce( *(genus->balancing) );// equilibria used for balancing
                populate(roaming,genus->prod_only->head);
                populate(roaming,genus->reac_only->head);
                populate(balance,genus->delimited->head);

                const alist             &act = *alive;
                lexicon_type             sto;
                solo_repo<const species> zap(act->size);

                for(const collector::entry *ep=cw->head;ep;ep=ep->next)
                {
                    //----------------------------------------------------------
                    // get native weights from RAVEn
                    //----------------------------------------------------------
                    const readable<int> &native = *ep;

                    //----------------------------------------------------------
                    // expand native weights to global weights
                    //----------------------------------------------------------
                    eDict->expand(weight,native);


                    //----------------------------------------------------------
                    // register all used species by their INDEX
                    //----------------------------------------------------------
                    store(sto,weight,Nu);

                    //----------------------------------------------------------
                    // derive the new stoichiometry
                    //----------------------------------------------------------
                    qbranch::assess(stoich, weight, Nu);

                    //----------------------------------------------------------
                    // check one or more species disappeared
                    //----------------------------------------------------------
                    zap.clear();
                    for(const anode *an=act->head;an;an=an->next)
                    {
                        const species &s = an->host;
                        const size_t   j = *s;
                        if( (0==stoich[j]) && sto.search(j) ) zap << s;
                    }

                    if(zap.size)
                    {
                        std::cerr << "[usefull]" << zap << std::endl;
                    }
                    else
                    {
                        std::cerr << "[useless]" << std::endl;
                        continue;
                    }


                    //----------------------------------------------------------
                    // update system
                    //----------------------------------------------------------
                    const equilibrium &eq = promote_mixed(weight,stoich,K,lib,eqs,all);  // create mixed equlibrium
                    const size_t       dg = qselect::count_valid(weight); assert(dg>=2); // degree
                    layout.degree(dg) << eq;                                             // register degree
                    coerce(*group)    << eq;                                             // append to group
                    if( coerce(*genus).dispatch((*group)->tail) ) roaming.ensure(&eq);   // classifying and updating
                    if( !has_roaming(native, (*group)->head,roaming) ) balance << eq;    // check if kept for balance
                    coerce(*static_cast<const sp_repo *>(eq.info)).swap_with(zap);       // assign zeroed species
                }

            }

            
            if(xml.verbose)
            {
                YACK_XMLSUB(xml,"hierarchy");
                for(size_t i=1;i<=cross->size();++i) {
                    const eq_repo &er    = *((*cross)[i]);
                    const string   level = vformat("|level#%u|=%u",unsigned(i),unsigned(er.size));
                    YACK_XMLSUB(xml,level);

                    // table of textual components
                    vector<string> strings(er.size,as_capacity);
                    size_t         str_max = 0;
                    for(const eq_node *node=er.head;node;node=node->next)
                    {
                        const components  &cm = ***node;
                        strings << cm.to_string();
                        str_max = max_of(str_max,strings.back().size());
                    }

                    size_t istr = 1;
                    for(const eq_node *node=er.head;node;node=node->next,++istr)
                    {
                        const equilibrium &eq = ***node;
                        const string      &es = strings[istr];
                        all.pad(*xml << eq.name, eq) << " : " << strings[istr];
                        if(i>1)
                        {
                            for(size_t j=es.size();j<=str_max;++j) std::cerr << ' ';
                            std::cerr << "| \\ " << *static_cast<const sp_repo *>(eq.info);
                        }
                        std::cerr << std::endl;
                    }
                }

                {
                    YACK_XMLSUB(xml, "balancing");
                    for(const eq_node *node=genus->balancing->head;node;node=node->next)
                    {
                        YACK_XMLOG(xml, " (*) <" << (***node).name << ">");
                    }
                }
            }

        }
        
    }
    
}

