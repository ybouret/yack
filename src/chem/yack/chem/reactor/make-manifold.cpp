#include "yack/chem/reactor/cluster.hpp"
#include "yack/system/imported.hpp"
#include "yack/apex/alga.hpp"
#include "yack/raven/qbranch.hpp"
#include "yack/raven/qselect.hpp"
#include "yack/sequence/bunch.hpp"

namespace yack
{
    using namespace raven;

    namespace chemical
    {
        namespace {

            // select more than two coefficients
            template <typename T> static inline
            bool keep_more_than_two(const readable<T> &cf) throw()
            {
                return qselect::count_valid(cf) >= 2;
            }

            //! local collector
            class collector : public bunch<int>
            {
            public:
                explicit collector(const size_t w) : bunch<int>(w) {}
                virtual ~collector() throw() {}

                void operator()(const qvector &cf)
                {
                    if( qselect::count_valid(  cf.cast_to(work) ) >= 2 )
                    {
                        ensure(work);
                    }
                }

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

                inline void organize()
                {
                    sort_with(compare);
                }


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(collector);
            };

        }


        static inline
        void compute_local_topology(matrix<int>       &nu,
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

                //--------------------------------------------------------------
                // compute local topology and its transposed
                //--------------------------------------------------------------
                {
                    matrix<int>  nu(n,m);
                    compute_local_topology(nu,grp,act,Nu);
                    YACK_XMLOG(xml, "nu=" << nu);
                    if(n!=alga::rank(nu))            throw imported::exception(cluster::clid,"invalid topology rank");
                    if(n!=qselect::compress(mu,nu))  throw imported::exception(cluster::clid,"invalid transposed rank");
                    YACK_XMLOG(xml, "mu=" << mu);
                }

                //--------------------------------------------------------------
                // use RAVEn
                //--------------------------------------------------------------
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
                gvector       &gcross = coerce( *cross );
                cxx_array<int> weight(Nu.rows);
                cxx_array<int> stoich(Nu.cols);
                {
                    size_t imix=1;
                    for(const collector::entry *ep=cw->head;ep;ep=ep->next,++imix)
                    {
                        const readable<int> &native = *ep;                                   // native weights
                        eDict->expand(weight,native);                                        // global weights
                        qbranch::assess(stoich, weight, Nu);                                 // derive stoich coefficients
                        const equilibrium &eq = promote_mixed(weight,stoich,K,lib,eqs,all);  // create mixed equlibrium
                        const size_t       dg = qselect::count_valid(weight);                // degree
                        gcross.degree(dg) << eq;                                             // register degree





                        const components  &cm = eq;
                        std::cerr << "u" << imix << " : |" << native << "|=" << dg << " => " << stoich << " : " << eq.name << " : " << cm << std::endl;
                        coerce(*group) << eq;
                    }
                }
            }




        }
        
    }
    
}

