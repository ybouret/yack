
//! \file

#ifndef YACK_APEX_NUMBER_INCLUDED
#define YACK_APEX_NUMBER_INCLUDED 1

#include "yack/object.hpp"

namespace yack
{

    namespace apex
    {

        //______________________________________________________________________
        //
        //
        //! base class for apex
        //
        //______________________________________________________________________
        class number : public object
        {
        public:
            virtual ~number() throw(); //!< cleanup

        protected:
            explicit number() throw(); //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(number);
        };
    }

}

#endif
