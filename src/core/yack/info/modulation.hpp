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
            virtual ~modulation() throw(); //!< cleanup

            virtual void    reset()              throw() = 0; //!< reset state
            virtual uint8_t alter(const uint8_t) throw() = 0; //!< alter byte

        protected:
            explicit modulation() throw(); //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(modulation);

        public:
            static uint8_t call(const uint8_t, void *) throw(); //!< for C code
            void          *self() throw();                      //!< for C code
        };

    }
}

#endif

