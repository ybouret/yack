//! \file

#ifndef YACK_IOS_CHARACTER_INCLUDED
#define YACK_IOS_CHARACTER_INCLUDED 1

#include "yack/memory/small/exclusive.hpp"


namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! dynamic character with dedicated allocator
        //
        //______________________________________________________________________
        class character
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            YACK_EXCLUSIVE_DECL(character,32700); //!< aliases
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            character(const uint8_t ch) throw();               //!< create
            character()  throw();                              //!< create with 0
            ~character() throw();                              //!< cleanup
            character(const character &) throw();              //!< copy code
            character & operator=( const character &) throw(); //!< assign code
            character & operator=( const uint8_t    ) throw(); //!< assign code
            
            //__________________________________________________________________
            //
            // access
            //__________________________________________________________________
            uint8_t       & operator*()       throw(); //!< access
            const uint8_t & operator*() const throw(); //!< access
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            character *next; //!< for list/pool
            character *prev; //!< for list
            uint8_t    code; //!< data
            
        };

       


       
    
        
    }
}

#endif


