#include "yack/aqueous/realm/domain.hpp"
#include "yack/system/imported.hpp"
#include "yack/apex/alga.hpp"

namespace yack
{
    namespace aqueous
    {

        void domain:: create_topology(const xmlog &xml)
        {
            //------------------------------------------------------------------
            // compute topology matrix
            //------------------------------------------------------------------
            {
                matrix<int> &Nu_ = coerce(Nu);
                Nu_.make(N,M);
                for(const eq_node *en=head;en;en=en->next)
                {
                    const equilibrium &eq = ***en;
                    eq.fill(Nu_[eq.indx[1]],1);
                }
            }

            const size_t rank = alga::rank(Nu);
            if( rank < N ) throw imported::exception(clid,"rank=%u < %u, check dependencies!!", unsigned(rank), unsigned(N) );


            //------------------------------------------------------------------
            // deduce transposed
            //------------------------------------------------------------------
            coerce(NuT).make(M,N);
            coerce(NuT).assign(Nu,transposed);
            YACK_XMLOG(xml, "Nu   = " << Nu);
        }

    }

}

