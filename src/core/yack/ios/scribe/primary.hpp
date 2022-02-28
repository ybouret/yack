
//! \file

#ifndef YACK_IOS_SCRIBE_PRIMARY_INCLUDED
#define YACK_IOS_SCRIBE_PRIMARY_INCLUDED 1

#include "yack/ios/scribe.hpp"
#include "yack/string.hpp"

namespace yack
{
    namespace ios
    {

        class primary_scribe : public scribe
        {
        public:
            virtual ~primary_scribe() throw();
            explicit primary_scribe(const rtti &, const char *ini);

            string   fmt;



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(primary_scribe);
        };

        
    }

}

#endif
