
//! \file

#ifndef YACK_JIVE_CHARACTER_INCLUDED
#define YACK_JIVE_CHARACTER_INCLUDED 1

#include "yack/jive/context.hpp"
#include "yack/memory/small/exclusive.hpp"

namespace yack
{
    namespace jive
    {
        //______________________________________________________________________
        //
        //
        //! character with full info
        //
        //______________________________________________________________________
        class character : public context
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            YACK_EXCLUSIVE_DECL(character,1); //!< aliases

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~character() throw();                               //!< cleanup
            explicit character(const context &, const uint8_t) throw(); //!< no-throw build
            character(const character &) throw();                       //!< no-throw copy

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const uint8_t & operator*() const throw(); //!< access

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            character    *next; //!< for list/pool
            character    *prev; //!< for list
            const uint8_t code; //!< the code

        private:
            YACK_DISABLE_ASSIGN(character);
        };

    }

}

#endif
