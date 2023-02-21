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
            typedef uint8_t                 type; //!< alias
            typedef uint8_t           const_type; //!< alias
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            character(const uint8_t ch) noexcept;               //!< create
            character()  noexcept;                              //!< create with 0
            ~character() noexcept;                              //!< cleanup
            character(const character &) noexcept;              //!< copy code
            character & operator=( const character &) noexcept; //!< assign code
            character & operator=( const uint8_t    ) noexcept; //!< assign code
            
            //__________________________________________________________________
            //
            // access
            //__________________________________________________________________
            uint8_t       & operator*()       noexcept; //!< access
            const uint8_t & operator*() const noexcept; //!< access
            
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


