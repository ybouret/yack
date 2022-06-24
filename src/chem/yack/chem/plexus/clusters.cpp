
#include "yack/chem/plexus.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/counting/comb.hpp"
#include "yack/ptr/auto.hpp"
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {

        namespace {

            static inline void update(clusters &attached, const equilibrium &lhs)
            {
                // check is attached
                for(cluster *cls=attached.head;cls;cls=cls->next)
                {
                    assert( false == cls->carries(lhs) );
                    if(cls->accepts(lhs))
                    {
                        // ok, insert it
                        (*cls) << &lhs;
                        return;
                    }
                }

                // new cluster here
                attached.create_from(lhs);
            }

        }


        namespace {

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
                        if(!ok[*ej]) return false;
                    }
                }
                return true;
            }

            static inline
            void process(clusters           &born,
                         const equilibrium  &host,
                         const cluster      &star,
                         const matrix<bool> &detached)
            {

                // fetch number of candidates
                const size_t n = star.size;
                if(n<=0)
                {
                    //----------------------------------------------------------
                    // only single
                    //----------------------------------------------------------
                    born.create_from(host);
                    //std::cerr << "\tadding single " << host.name << std::endl;
                }
                else
                {
                    //----------------------------------------------------------
                    // at least one combination: extract guest from star
                    //----------------------------------------------------------
                    vector<equilibrium *> guest(n,as_capacity);
                    vector<equilibrium *> party(n,as_capacity);
                    for(const vnode *node=star.head;node;node=node->next)
                    {
                        const equilibrium &tmp = **node;
                        guest << (equilibrium * )&tmp;
                    }

                    //----------------------------------------------------------
                    // try combinations
                    //----------------------------------------------------------
                    for(size_t k=n;k>0;--k)
                    {
                        combination           comb(n,k);
                        //std::cerr << "\ttesting (" << n << "," << k << ") = #" << comb.total << std::endl;
                        do
                        {
                            comb.extract(party,guest);
                            if( are_detached(party,detached) )
                            {
                                // create a new cluster
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
                                std::cerr << "\t" << cc << std::endl;
                                born.push_back( cc.yield() );
                            }


                        } while(comb.next());
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
            // precompute detached matrix and top-level clusters
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
            // compute detached cluster per topLevel
            //
            //------------------------------------------------------------------
            const size_t        dims = attached.size;
            cxx_array<clusters> part(dims);
            

            for(const enode *node = lattice.head(); node; node=node->next )
            {
                const equilibrium    &host = ***node;
                const size_t          indx = *host;
                const size_t          info = host.info;
                const readable<bool> &flag = detached[indx];
                cluster               star;

                for(const enode *scan=node->next;scan;scan=scan->next)
                {
                    const equilibrium &rhs = ***scan;
                    if(rhs.info!=info) continue;
                    if( flag[*rhs] ) star << &rhs;
                }

                lattice.pad(std::cerr << host.name,host) << " : $" << std::setw(3) << info << " : " << star << std::endl;

                process(part[host.info],host,star,detached);

            }
            std::cerr << std::endl;
            std::cerr << part << std::endl;
            std::cerr << std::endl;

            exit(1);
        }
    }

}


