
#include "yack/aqueous/realm/domain.hpp"
#include "yack/system/imported.hpp"
#include "yack/apex/alga.hpp"

namespace yack
{
    namespace aqueous
    {

        const char * const domain::clid = "aqueous::domain";

        void domain:: create(const xmlog &xml)
        {
            YACK_XMLSUB(xml,"create_domain");
            YACK_XMLOG(xml, "here = " << *this);
            assert(size>0);
            make_dimensions(xml);
            matrix<int> alpha;
            create_topology(xml,alpha);
            build_conserved(xml,alpha);
        }
    }

}


