

//! \file

#ifndef YACK_IOS_SCRIBE_REPOSITORY_INCLUDED
#define YACK_IOS_SCRIBE_REPOSITORY_INCLUDED 1

#include "yack/ios/scribe.hpp"
#include "yack/singleton.hpp"

namespace yack
{
    namespace ios
    {

        class scribes : public singleton<scribes>
        {
        public:
            static const char               call_sign[];
            static const at_exit::longevity life_time = 7;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(scribes);
            friend class singleton<scribes>;
            explicit scribes();
            virtual ~scribes() throw();

        };

    }

}

#endif

