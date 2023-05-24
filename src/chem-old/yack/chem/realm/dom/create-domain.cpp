#include "yack/chem/realm/domain.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace chemical
    {

        const char * const domain::clid = "chemical::domain";

        void domain:: create(const xmlog            &xml,
                             equilibria             &eqs,
                             const readable<double> &eks)
        {
            YACK_XMLSUB(xml,"create_domain");
            YACK_XMLOG(xml, "here = " << *this);
            assert(size>0);
            make_dimensions(xml);
            create_topology(xml);
            create_manifold(xml,eqs,eks);
            make_index_maps(xml);
            
            make_categories(xml);
            build_conserved(xml);
            make_partitions(xml);
        }
    }

}


