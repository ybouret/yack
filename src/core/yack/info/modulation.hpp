//! \file

#ifndef YACK_INFO_MODULATION_INCLUDED
#define YACK_INFO_MODULATION_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/info/modulation.h"

namespace yack
{

    namespace information
    {

        //______________________________________________________________________
        //
        //
        //! byte-wise modulation base class
        //
        //______________________________________________________________________
        class modulation
        {
        public:
            virtual ~modulation() noexcept; //!< cleanup

            virtual void    reset()              noexcept = 0; //!< reset state
            virtual uint8_t alter(const uint8_t) noexcept = 0; //!< alter byte

        protected:
            explicit modulation() noexcept; //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(modulation);

        public:
            static uint8_t call(const uint8_t, void *) noexcept; //!< for C code
            void          *self() noexcept;                      //!< for C code
        };

    }
}

#endif

