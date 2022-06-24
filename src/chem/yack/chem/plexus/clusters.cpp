
#include "yack/chem/plexus.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/counting/comb.hpp"
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
            // update attached clusters with a new equilibrium:
            // insert it in an  accepting cluster
            // or create a new cluster
            //
            //------------------------------------------------------------------
            static inline void update(clusters          &attached,
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
                        return;         // and we're done
                    }
                }

                //--------------------------------------------------------------
                // need tp create a new cluster at this point
                //--------------------------------------------------------------
                attached.createFrom(lhs);
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
                return true;
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
                    //
                    //----------------------------------------------------------
                    vector<equilibrium *> guest(n,as_capacity);
                    vector<equilibrium *> party(n,as_capacity);
                    for(const vnode *node=star.head;node;node=node->next)
                    {
                        const equilibrium &tmp = **node;
                        guest << (equilibrium * )&tmp;
                    }

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
                            cc->update(); assert(cc->isValid());
                            //std::cerr << "\t" << cc << std::endl;
                            born.push_back( cc.yield() );
                        } while(comb.next());
                    }

                }
            }

        }

        static inline void finalizeClusters(clusters &born)
        {
            //------------------------------------------------------------------
            //
            // sort by size
            //
            //------------------------------------------------------------------
            born.sort();

            //------------------------------------------------------------------
            //
            // check no redundancy, keeping the longest cluster
            //
            //------------------------------------------------------------------
            clusters temp;
            while(born.size)
            {
                //--------------------------------------------------------------
                // remove first item
                //--------------------------------------------------------------
                auto_ptr<cluster>  cc=born.pop_front();

                //--------------------------------------------------------------
                // sanity check
                //--------------------------------------------------------------
                for(const cluster *sc=born.head;sc;sc=sc->next)
                {
                    if( cc->matches( *sc ) )
                    {
                        throw exception("multiple clusters");
                    }
                }

                //--------------------------------------------------------------
                // search bigger cluster including cc
                //--------------------------------------------------------------
                bool found = false;
                for(const cluster *sc=born.head;sc;sc=sc->next)
                {
                    if(sc->includes(*cc))
                    {
                        //std::cerr << cc << " included by " << *sc << std::endl;
                        found = true;
                        break;
                    }
                }

                //--------------------------------------------------------------
                // keep if not found
                //--------------------------------------------------------------
                if(!found) temp.push_back( cc.yield() );
            }

            born.swap_with(temp);

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
            clusters     attached;
            for(const enode *node = lattice.head(); node; node=node->next )
            {
                const equilibrium &lhs = ***node;
                update(attached,lhs);
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

                for(const enode *scan=node->next;scan;scan=scan->next)
                {
                    const equilibrium &rhs = ***scan;
                    if(rhs.info!=info) continue;
                    if( flag[*rhs] ) star << &rhs;
                }

                lattice.pad(std::cerr << host.name,host) << " : $" << std::setw(3) << info << " : " << star << std::endl;

                //--------------------------------------------------------------
                //
                // update clusters in host's partion
                //
                //--------------------------------------------------------------
                process(part[host.info],host,star,detached);
            }

            std::cerr << std::endl;
            std::cerr << part << std::endl;
            std::cerr << std::endl;

            //------------------------------------------------------------------
            //
            // finalize partitions by sorting and removing redundancy
            //
            //------------------------------------------------------------------
            for(size_t i=dims;i>0;--i)
            {
                finalizeClusters(part[i]);
            }
            std::cerr << part << std::endl;
            std::cerr << std::endl;

            //------------------------------------------------------------------
            //
            // make final combinations
            //
            //------------------------------------------------------------------
            


            exit(1);
        }
    }

}


