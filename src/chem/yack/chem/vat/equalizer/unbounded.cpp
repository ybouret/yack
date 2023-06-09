
#include "yack/chem/vat/equalizer.hpp"
#include "yack/system/imported.hpp"
#include "yack/math/keto.hpp"
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace Chemical
    {

        void Equalizer:: runUbounded(const xmlog               &xml,
                                      writable<Extended::Real> &C0,
                                      const Cluster            &cluster)
        {
            YACK_XMLSUB(xml,"Equalizer::runUbounded");
            assert(cluster.lib.size == (***cluster.lib.tail).indx[SubLevel]);

            //------------------------------------------------------------------
            //
            // adjusting workspace
            //
            //------------------------------------------------------------------
            const size_t m = cluster.lib.size;
            Corg.adjust(m,_0);
            //Ctmp.adjust(m,_0);
            //Cend.adjust(m,_0);
            cluster.load(Corg,C0);
            

        }

    }

}


