

//! \file

#ifndef YACK_IOS_SCRIBE_REPOSITORY_INCLUDED
#define YACK_IOS_SCRIBE_REPOSITORY_INCLUDED 1

#include "yack/ios/scribe.hpp"
#include "yack/singleton.hpp"
#include "yack/ptr/arc.hpp"
#include "yack/data/suffix/tree.hpp"
#include "yack/string.hpp"

namespace yack
{
    namespace ios
    {

        class scribes : public singleton<scribes>
        {
        public:
            static const char               call_sign[];
            static const at_exit::longevity life_time = 7;
            typedef arc_ptr<scribe>                         scribe_ptr;
            typedef kernel::suffix_tree<scribe_ptr,uint8_t> scribe_set;

            string real_format;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(scribes);
            friend class singleton<scribes>;
            explicit scribes();
            virtual ~scribes() throw();
            void use(scribe *);

            scribe_set db;

        };

    }

}

#endif

