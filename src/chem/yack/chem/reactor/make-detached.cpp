
#include "yack/chem/reactor/cluster.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace chemical
    {

        void cluster:: make_detached(const xmlog &xml)
        {
            static const char * const fn = "cluster::make_detached";
            YACK_XMLSUB(xml,fn);
            
        }
    }

}
