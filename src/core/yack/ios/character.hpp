//! \file

#ifndef YACK_IOS_CHARACTER_INCLUDED
#define YACK_IOS_CHARACTER_INCLUDED 1

#include "yack/memory/small/exclusive.hpp"
#include "yack/data/list/concrete.hpp"
#include "yack/data/pool/cxx.hpp"

namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! dynamic character with dedicater allocator
        //
        //______________________________________________________________________
        class character
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            YACK_EXCLUSIVE_DECL(character,0); //!< aliases
            
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
        
        typedef klist<uint8_t,character> characters_;

        class characters : public characters_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            characters() throw();
            virtual ~characters() throw();
            characters(const characters &);
            characters & operator=(const characters &);

            void add(const void *addr, size_t size);
            characters & operator<<(const char *msg);

        };



        //______________________________________________________________________
        //
        //
        //! pool for local cache
        //
        //______________________________________________________________________
        typedef cxx_pool_of<character> char_cache;
        
    
        
    }
}

#endif


