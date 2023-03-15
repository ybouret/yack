
#include "yack/aqueous/realm/domain.hpp"
#include "yack/system/imported.hpp"
#include "yack/apex/alga.hpp"

namespace yack
{
    namespace aqueous
    {

        const char * const domain::clid = "aqueous::domain";

        void domain:: create(const xmlog            &xml,
                             const library          &lib,
                             equilibria             &eqs,
                             const readable<double> &eks)
        {
            YACK_XMLSUB(xml,"create_domain");
            YACK_XMLOG(xml, "here = " << *this);
            assert(size>0);
            make_dimensions(xml);
            create_topology(xml);
            create_manifold(xml,lib,eqs,eks);
        }
    }

}


