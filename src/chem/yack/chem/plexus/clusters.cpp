
#include "yack/chem/plexus.hpp"
//#include "yack/data/list/sort.hpp"
#include "yack/sequence/cxx-array.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {


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
#if 1
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

            for(const enode *node = lattice.head(); node; node=node->next )
            {
                const equilibrium    &host = ***node;
                const size_t          indx = *host;
                const readable<bool> &flag = detached[indx];
                cluster               star;
                for(const enode *scan=node->next;scan;scan=scan->next)
                {
                    const equilibrium &rhs = ***scan;
                    if( flag[*rhs] ) star << &rhs;
                }

                lattice.pad(std::cerr << host.name,host) << " : " << star << std::endl;

            }


#endif




#if 0
            cxx_array<cluster> detached(Nl);
            for(const enode *node = lattice.head(); node; node=node->next )
            {
                const equilibrium &lhs = ***node;
                cluster           &cls = detached[*lhs];
                //for(const enode *scan=node;scan;scan=scan->next)
                for(const enode *scan=lattice.head();scan;scan=scan->next)
                {
                    const equilibrium &rhs = ***scan;
                    if(lhs.detached(rhs))
                    {
                        cls << &rhs;
                    }
                }
            }
            lattice(std::cerr,detached,"");
#endif

            exit(1);
        }
    }

}


