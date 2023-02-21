
//! \file

#ifndef YACK_INFO_MODULATION_ECHO_INCLUDED
#define YACK_INFO_MODULATION_ECHO_INCLUDED 1

#include "yack/info/modulation.hpp"


namespace yack
{

    namespace information
    {
        //______________________________________________________________________
        //
        //
        //! echo codec
        //
        //______________________________________________________________________
        class echo : public modulation
        {
        public:
            virtual ~echo() noexcept; //!< cleanup
            explicit echo() noexcept; //!< setup

            virtual void    reset()              noexcept; //!< do noting
            virtual uint8_t alter(const uint8_t) noexcept; //!< identity

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(echo);
        };
    }
}

#endif
