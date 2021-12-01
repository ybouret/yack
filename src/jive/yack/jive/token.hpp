//! \file

#ifndef YACK_JIVE_TOKEN_INCLUDED
#define YACK_JIVE_TOKEN_INCLUDED 1

#include "yack/jive/character.hpp"
#include "yack/data/list/concrete.hpp"
#include "yack/data/pool/concrete.hpp"

namespace yack
{
    namespace jive
    {

        typedef klist<uint8_t,character> characters; //!< base class for token
        typedef kpool<uint8_t,character> repository; //!< bool of tokens

        //______________________________________________________________________
        //
        //
        //! character with full info
        //
        //______________________________________________________________________
        class token : public characters
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~token() throw();  //!< cleanup
            explicit token() throw();  //!< no-throw build
            token(const token &);      //!< char-wise copy

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! display
            inline friend std::ostream & operator<< (std::ostream &os, const token &t)
            {
                return t.display(os);
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            token        *next; //!< for list/pool
            token        *prev; //!< for list

        private:
            YACK_DISABLE_ASSIGN(token);
        };

    }

}

#endif
