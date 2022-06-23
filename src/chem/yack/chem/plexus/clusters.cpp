
#include "yack/chem/plexus.hpp"
#include "yack/counting/comb.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/data/list/sort.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {



        static inline bool are_detached(const readable<equilibrium *> &v,
                                        const matrix<bool>            &detached) throw()
        {
            for(size_t i=v.size();i>1;--i)
            {
                const equilibrium    &lhs = *v[i];
                const readable<bool> &ok  = detached[ *lhs ];
                for(size_t j=i-1;j>0;--j)
                {
                    const equilibrium &rhs = *v[j];
                    if( !ok[*rhs] )
                    {
                        return false;
                    }
                }
            }
            return true;
        }


        static inline
        void  populate(clusters                &born,
                       const equilibrium       &host,
                       writable<equilibrium *> &guest,
                       const matrix<bool>      &detached)
        {
            //------------------------------------------------------------------
            //
            // sanity check
            //
            //------------------------------------------------------------------
            assert( 1== born.size );
            assert( &host == & **(born.head->head) );
            assert( guest.size() > 0);

            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            const size_t          n = guest.size();
            vector<equilibrium *> fragment(n,as_capacity);

            //------------------------------------------------------------------
            //
            // try all sub combinations in DECREASING size order
            //
            //------------------------------------------------------------------
            for(size_t k=1;k<=n;++k)
            {
                combination comb(n,k); // create combinations
                do
                {
                    //----------------------------------------------------------
                    // extract fragment from guests
                    //----------------------------------------------------------
                    comb.extract(fragment,guest); assert(k==fragment.size());

                    //----------------------------------------------------------
                    // check fully detached fragment
                    //----------------------------------------------------------
                    if(!are_detached(fragment,detached)) continue;

                    //----------------------------------------------------------
                    // create a new cluster and populate it
                    //----------------------------------------------------------
                    auto_ptr<cluster> cc = new cluster();

                    *cc << &host;
                    for(size_t i=1;i<=k;++i)
                    {
                        *cc << fragment[i];
                        assert( cc->carries( *fragment[i]) );
                    }

                    cc->update();
                    
                    //----------------------------------------------------------
                    // check that this cluster is not a sub-cluster
                    //----------------------------------------------------------

#if !defined(NDEBUG)
                    for(const cluster *tmp=born.head;tmp;tmp=tmp->next)
                    {
                        assert( ! tmp->matches(*cc) );
                    }
#endif
                    
                    born.push_back( cc.yield() );

                } while( comb.next() );
            }



        }



        static inline int compare_clusters(const cluster *lhs,
                                           const cluster *rhs) throw()
        {

            assert(lhs);
            assert(rhs);
            const size_t lsize = lhs->size;
            const size_t rsize = rhs->size;

            // check sizes
            switch(__sign::of(lsize,rsize))
            {
                case negative: assert(lsize<rsize); return -1;
                case positive: assert(rsize<lsize); return  1;
                case __zero__:
                    break;
            }


            // lexicographic order for same sizes
            size_t i = lsize;
            for(const vnode *L=lhs->head, *R=rhs->head;i>0;--i,L=L->next,R=R->next)
            {
                const size_t il = ***L;
                const size_t ir = ***R;
                switch(__sign::of(il,ir))
                {
                    case negative: assert(il<ir); return -1;
                    case positive: assert(ir<il); return  1;
                    case __zero__: break;
                }
            }

            // shouldn't happen here since all clusters are different
            return 0;
        }

        void  plexus:: makeReactiveClusters()
        {

            //------------------------------------------------------------------
            //
            // building clusters
            //
            //------------------------------------------------------------------
            YACK_CHEM_MARKUP( vpfx, "makeReactiveClusters");


            //------------------------------------------------------------------
            //
            // precompute detached matrix
            //
            //------------------------------------------------------------------
            matrix<bool> detached(Nl,Nl);
            for(const enode *node = lattice.head(); node; node=node->next )
            {
                const equilibrium &lhs = ***node;
                for(const enode *scan=node;scan;scan=scan->next)
                {
                    const equilibrium &rhs = ***scan;
                    detached[*lhs][*rhs] = detached[*rhs][*lhs] = lhs.detached(rhs);
                }
            }

            lattice(std::cerr,detached,"");

            exit(1);
            
            clusters &all = coerce(cls);

            //------------------------------------------------------------------
            //
            // check each possible equilibria in lattice
            //
            //------------------------------------------------------------------
            {
                vector<equilibrium *> guest(Nl,as_capacity);
                for(const enode *node = lattice.head(); node; node=node->next )
                {
                    //----------------------------------------------------------
                    // initialize
                    //----------------------------------------------------------
                    const equilibrium &host = ***node;  // local host
                    clusters           born;            // local clusters

                    born.create_from(host);

                    std::cerr << host.name;

                    //----------------------------------------------------------
                    // load all detached guest
                    //----------------------------------------------------------
                    guest.free();
                    for(const enode *scan=node->next;scan;scan=scan->next)
                    {
                        const equilibrium &curr = ***scan;
                        if(curr.detached(host))
                        {
                            guest << (equilibrium *) &curr;
                            std::cerr << '/' << curr.name;
                        }
                    }
                    const size_t n = guest.size();
                    std::cerr << " (" << host.name << " +" << n << ")" << std::endl;

                    //----------------------------------------------------------
                    // populate with possible clusters
                    //----------------------------------------------------------
                    if(n>0) populate(born,host,guest,detached);
                    std::cerr << born << std::endl;
                    all.merge_back(born);
                }
            }



            merge_list_of<cluster>::sort(all,compare_clusters);

            std::cerr << "All Possible Clusters:" << std::endl;
            std::cerr << all << std::endl;

            //------------------------------------------------------------------
            //
            // cleanup
            //
            //------------------------------------------------------------------



            exit(1);

        }
    }

}


