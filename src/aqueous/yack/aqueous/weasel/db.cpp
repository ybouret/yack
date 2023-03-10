
#include "yack/aqueous/weasel/designer.hpp"

namespace yack
{
    namespace aqueous
    {

        namespace weasel
        {

            const char * const designer::edb[] =
            {
#include "db.hxx"
            };

            const size_t designer:: ndb = sizeof(edb) / sizeof(edb[0]);

        }

    }

}

