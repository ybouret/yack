//! \file

#ifndef YACK_IOS_SCRIBE_INCLUDED
#define YACK_IOS_SCRIBE_INCLUDED 1

#include "yack/system/rtti.hpp"

namespace yack
{
    namespace ios
    {

        class scribe : public object, public counted
        {
        public:
            virtual ~scribe() throw();
            const rtti      &tid;

            virtual string   to_string(const void *addr) const = 0;

        protected:
            explicit scribe(const rtti &) throw();



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(scribe);
        };

    }
}


#endif

