
#include "yack/chem/weasel/designer.hpp"

namespace yack
{
    namespace chemical
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

