
#include "yack/ios/scribe/primary.hpp"

namespace yack
{
    namespace ios
    {

        primary_scribe:: ~primary_scribe() throw()
        {
        }

        primary_scribe:: primary_scribe(const rtti &uid, const char *ini) :
        scribe(uid),
        fmt(ini)
        {
        }

        
    }
}

