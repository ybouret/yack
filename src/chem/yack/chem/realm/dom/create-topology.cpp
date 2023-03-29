#include "yack/chem/realm/domain.hpp"
#include "yack/system/imported.hpp"
#include "yack/apex/alga.hpp"

namespace yack
{
    namespace chemical
    {

        
        void domain:: create_topology(const xmlog &xml)
        {
          
            YACK_XMLSUB(xml,"create_topology");


            //------------------------------------------------------------------
            // compute topology matrix
            //------------------------------------------------------------------
            {
                matrix<int> &Nu_ = coerce(Nu);
                Nu_.make(N,M);
                for(const eq_node *en=head;en;en=en->next)
                {
                    const equilibrium &eq = ***en;
                    const size_t       ei = eq.indx[1];
                    writable<int>     &nu = Nu_[ei];
                    eq.fill(sub_level,nu);
                }
            }

            {
                const size_t rank = alga::rank(Nu);
                if( rank < N ) throw imported::exception(clid,"rank=%u < %u, check dependencies!!", unsigned(rank), unsigned(N) );
            }

            //------------------------------------------------------------------
            // deduce transposed
            //------------------------------------------------------------------
            coerce(NuT).make(M,N);
            coerce(NuT).assign(Nu,transposed);
            YACK_XMLOG(xml, "Nu   = " << Nu);

        }

    }

}

