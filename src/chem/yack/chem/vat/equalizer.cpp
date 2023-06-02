
#include "yack/chem/vat/equalizer.hpp"

namespace yack
{
    namespace Chemical
    {


        Equalizer:: ~Equalizer() noexcept
        {
        }

        Equalizer:: Equalizer()
        {
        }

        void Equalizer:: run(const xmlog      &xml,
                             writable<double> &C,
                             const Cluster    &cl)
        {
            YACK_XMLSUB(xml,"Equalizing");


            

        }
        
    }

}

