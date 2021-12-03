
//! \file

#ifndef YACK_JIVE_PATTERN_LOGICAL_BANK_INCLUDED
#define YACK_JIVE_PATTERN_LOGICAL_BANK_INCLUDED 1

#include "yack/jive/pattern/logic/logical.hpp"

namespace yack
{
    namespace jive
    {
        //______________________________________________________________________
        //
        //
        //! bank of patterns for OR/NONE
        //
        //______________________________________________________________________
        class bank : public logical
        {
        public:
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void     no_multiple() throw(); //!< remove multiple patterns

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~bank()               throw(); //!< cleanup
        protected:                                 //|    and
            explicit bank(const uint32_t) throw(); //!<  setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(bank);
        };

    }

}

#endif
