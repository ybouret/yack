
#include "yack/aqueous/realm/claw/custodian.hpp"

namespace yack
{
    namespace aqueous
    {
        custodian:: ~custodian() noexcept
        {
        }

        custodian:: custodian(const size_t M) :
        totalC(M),
        deltaC(M)
        {
        }

        void custodian:: prepare() noexcept
        {
            totalC.ld(0);
        }

        void custodian:: process(writable<double> &C,
                                 const conserved  &laws)
        {

            broken.clear();
            excess.clear();

            // initialize
            for(const conserved::node_type *node=laws.head;node;node=node->next)
            {
                const conservation &law = ***node;
                const double        cxs = law.excess(C,xadd);
                if(cxs<0)
                {
                    broken << law;
                    excess << cxs;
                }
            }

            // select smallest excess
            

        }



    }


}

