
//! \file

#ifndef YACK_APEX_NUMBER_INCLUDED
#define YACK_APEX_NUMBER_INCLUDED 1

#include "yack/object.hpp"

namespace yack
{

    namespace apex
    {

#define YACK_APEX_TRACKING 1

        //______________________________________________________________________
        //
        //
        //! base class for apex
        //
        //______________________________________________________________________
        class number : public object
        {
        public:
            static uint64_t add_ticks;
            static size_t   add_count;
            static uint64_t sub_ticks;
            static size_t   sub_count;
            static void     reset_tracking() throw();

            virtual ~number() throw(); //!< cleanup

        protected:
            explicit number() throw(); //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(number);
        };
    }

}

#endif
