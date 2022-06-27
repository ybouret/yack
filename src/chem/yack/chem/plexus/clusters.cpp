
#include "yack/chem/plexus.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/counting/comb.hpp"
#include "yack/counting/mloop.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/exception.hpp"

#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {

        namespace {

            //------------------------------------------------------------------
            //
            // dispatch a new equilibrium in attached clusters:
            // insert it in an  accepting cluster
            // or create a new cluster
            //
            //------------------------------------------------------------------
            static inline
            const equilibrium &  dispatch(clusters          &attached,
                                          const equilibrium &lhs)
            {

                //--------------------------------------------------------------
                // loop over existing clusters
                //--------------------------------------------------------------
                for(cluster *cls=attached.head;cls;cls=cls->next)
                {
                    assert( false == cls->carries(lhs) );
                    if(cls->accepts(lhs))
                    {
                        (*cls) << &lhs; // ok, insert it
                        return lhs;     // and we're done
                    }
                }

                //--------------------------------------------------------------
                // need to create a new cluster at this point
                //--------------------------------------------------------------
                attached.createFrom(lhs);
                return lhs;
            }


            //------------------------------------------------------------------
            //
            // check if a party of equilibria ate detached from one another
            //
            //------------------------------------------------------------------
            static inline bool are_detached(const readable<equilibrium *> &party,
                                            const matrix<bool>            &detached) throw()
            {
                for(size_t i=party.size();i>1;--i)
                {
                    const equilibrium    &ei = *party[i];
                    const readable<bool> &ok = detached[ *ei ];
                    for(size_t j=i-1;j>0;--j)
                    {
                        const equilibrium &ej = *party[j];
                        if(!ok[*ej]) return false; // overlapping detected
                    }
                }
                return true; // no overlapping
            }

            //------------------------------------------------------------------
            //
            // from a host equilibrium and a collection of detached from host
            // build all possible fully detached combinations
            //
            //------------------------------------------------------------------
            static inline
            void process(clusters           &born,
                         const equilibrium  &host,
                         const cluster      &star,
                         const matrix<bool> &detached)
            {

                //--------------------------------------------------------------
                //
                // fetch number of candidates
                //
                //------------------------------------------------------------------
                const size_t n = star.size;
                if(n<=0)
                {
                    //----------------------------------------------------------
                    //
                    // no detached equilibria => only single
                    //
                    //----------------------------------------------------------
                    born.createFrom(host);
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // at least one combination: extract guest from star
                    // to make a party
                    //
                    //----------------------------------------------------------
                    vector<equilibrium *> guest(n,as_capacity);
                    vector<equilibrium *> party(n,as_capacity);
                    for(const vnode *node=star.head;node;node=node->next)
                        guest << (equilibrium * )& **node;

                    //----------------------------------------------------------
                    //
                    // try combinations
                    //
                    //----------------------------------------------------------
                    for(size_t k=n;k>0;--k)
                    {
                        combination comb(n,k);
                        do
                        {
                            //--------------------------------------------------
                            // extract party
                            //--------------------------------------------------
                            comb.extract(party,guest);

                            //--------------------------------------------------
                            // check if fully detached
                            //--------------------------------------------------
                            if(!are_detached(party,detached)) continue;


                            //--------------------------------------------------
                            // create a new cluster
                            //--------------------------------------------------
                            auto_ptr<cluster> cc = new cluster();
                            (*cc) << &host;
                            for(size_t i=1;i<=k;++i)
                            {
                                const equilibrium &eq = *party[i];
                                assert(detached[*host][*eq]);
                                assert(detached[***(cc->tail)][*eq]);
                                assert(!cc->carries(eq));
                                (*cc) << &eq;
                            }

                            //--------------------------------------------------
                            // new cluster is ready
                            //--------------------------------------------------
                            cc->update();
                            assert(cc->isValid());
                            assert(cc->isOrtho());

                            //--------------------------------------------------
                            // append to current clusters
                            //--------------------------------------------------
                            born.push_back( cc.yield() );
                        } while(comb.next());
                    }

                }
            }



            static inline void finalizeClusters(clusters &born)
            {
                //--------------------------------------------------------------
                //
                // sort by size
                //
                //--------------------------------------------------------------
                born.sort();

                //--------------------------------------------------------------
                //
                // check no redundancy, keeping the longest cluster
                //
                //--------------------------------------------------------------
                clusters temp;
                while(born.size)
                {
                    //----------------------------------------------------------
                    // remove first item
                    //----------------------------------------------------------
                    auto_ptr<cluster>  cc=born.pop_front();

                    //----------------------------------------------------------
                    // sanity check
                    //----------------------------------------------------------
                    for(const cluster *sc=born.head;sc;sc=sc->next)
                    {
                        if( cc->matches( *sc ) )
                        {
                            throw exception("multiple clusters");
                        }
                    }

                    //----------------------------------------------------------
                    // search bigger cluster including cc
                    //----------------------------------------------------------
                    bool found = false;
                    for(const cluster *sc=born.head;sc;sc=sc->next)
                    {
                        if(sc->includes(*cc))
                        {
                            found = true;
                            break;
                        }
                    }

                    //----------------------------------------------------------
                    // keep if not found
                    //----------------------------------------------------------
                    if(!found) temp.push_back( cc.yield() );
                }

                born.swap_with(temp);
            }

            static inline void combineAllClusters(clusters                 &all,
                                                  const readable<clusters> &part)
            {
                const size_t dims = part.size();

                //--------------------------------------------------------------
                //
                // create ND loop
                //
                //--------------------------------------------------------------
                const vector<unit_t> ini(dims,1);
                vector<unit_t>       end(dims,0);
                for(size_t i=dims;i>0;--i)
                {
                    end[i] = static_cast<unit_t>( part[i].size );
                }
                mloop<unit_t> loop( ini(), end(), dims);

                //--------------------------------------------------------------
                //
                // assemble sub-parts
                //
                //--------------------------------------------------------------
                do
                {
                    cluster *target = all.push_back( new cluster() );
                    for(size_t i=1;i<=dims;++i)
                    {
                        const cluster &source = *part[i].get( loop[i] );
                        target->merge_back_copy(source);
                    }
                    target->update();
                    assert(target->isValid());
                    assert(target->isOrtho());
                } while( loop.next() );

                //--------------------------------------------------------------
                //
                // final sort
                //
                //--------------------------------------------------------------
                all.sort();
                for(const cluster *a=all.head;a;a=a->next)
                {
                    if(!a->isValid()) throw exception("%s: cluster is not sorted",plexus::clid);
                    if(!a->isOrtho()) throw exception("%s: cluster is not detached",plexus::clid);
                    for(const cluster *b=a->next;b;b=b->next)
                    {
                        if(a->matches(*b)) throw exception("%s: multiple finalized clusters!!", plexus::clid);
                    }
                }
            }

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
            // and clusters of attached equilbria
            //
            //------------------------------------------------------------------
            matrix<bool> detached(Nl,Nl);
            clusters    &attached = coerce(com);
            for(const enode *node = lattice.head(); node; node=node->next )
            {
                const equilibrium &lhs = dispatch(attached,***node);
                for(const enode *scan=node;scan;scan=scan->next)
                {
                    const equilibrium &rhs = ***scan;
                    detached[*lhs][*rhs] = detached[*rhs][*lhs] = lhs.detached(rhs);
                }
            }
            attached.normalize();
            
            lattice(std::cerr,detached,"");
            std::cerr << "attached=" << attached << std::endl;

            //------------------------------------------------------------------
            //
            // compute all partitions
            //
            //------------------------------------------------------------------
            const size_t        dims = attached.size;
            cxx_array<clusters> part(dims);

            for(const enode *node = lattice.head(); node; node=node->next )
            {
                //--------------------------------------------------------------
                //
                // fetch local state
                //
                //--------------------------------------------------------------
                const equilibrium    &host = ***node;         // host equilibrium
                const size_t          indx = *host;           // host index
                const size_t          info = host.info;       // host partition index
                const readable<bool> &flag = detached[indx];  // host detached state
                cluster               star;                   // detached equilibria from the SAME partition

                //--------------------------------------------------------------
                //
                // star all detached equilibria of the same partition
                //
                //--------------------------------------------------------------
                for(const enode *scan=node->next;scan;scan=scan->next)
                {
                    const equilibrium &rhs = ***scan;
                    if(rhs.info!=info) continue;
                    if( flag[*rhs] ) star << &rhs;
                }


                //--------------------------------------------------------------
                //
                // update clusters in host's partition
                //
                //--------------------------------------------------------------
                process(part[host.info],host,star,detached);
            }



            //------------------------------------------------------------------
            //
            // finalize partitions by sorting and removing redundancy
            //
            //------------------------------------------------------------------
            for(size_t i=dims;i>0;--i)
            {
                finalizeClusters(part[i]);
            }
            std::cerr << std::endl;
            std::cerr << part << std::endl;
            std::cerr << std::endl;

            //------------------------------------------------------------------
            //
            // make final combinations of unique clusters
            //
            //------------------------------------------------------------------
            combineAllClusters(coerce(com),part);

            std::cerr << com << std::endl;


            //------------------------------------------------------------------
            //
            // run-time check
            //
            //------------------------------------------------------------------
            for(const enode *node = lattice.head(); node; node=node->next )
            {
                const equilibrium &lhs = ***node;
                bool               found = false;
                for(const cluster *cls=com.head;cls;cls=cls->next)
                {
                    if(cls->carries(lhs))
                    {
                        found = true;
                        break;
                    }
                }
                if(!found) throw exception("%s: missing '%s' in clusters!!",clid,lhs.name());
            }





            exit(1);
        }
    }

}


