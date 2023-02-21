//! \file

#ifndef YACK_IOS_SCRIBE_INCLUDED
#define YACK_IOS_SCRIBE_INCLUDED 1

#include "yack/system/rtti.hpp"

namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! base class for a scribe: object to string
        //
        //______________________________________________________________________
        class scribe : public object, public counted
        {
        public:
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const rtti      &tid; //!< global rtti, acts as key

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            virtual       ~scribe() noexcept;                      //!< cleanup
            virtual string to_string(const void *addr) const = 0; //!< interface

        protected:
            explicit scribe(const rtti &) noexcept; //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(scribe);
        };

    }
}


#endif

