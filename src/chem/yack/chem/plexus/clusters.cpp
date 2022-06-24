
#include "yack/chem/plexus.hpp"
#include "yack/sequence/cxx-array.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {

        namespace
        {
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

                lattice.pad(std::cerr << host.name,host) << " : " << star << " $" << info << std::endl;

            }



            exit(1);
        }
    }

}


