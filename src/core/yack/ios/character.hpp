//! \file

#ifndef YACK_IOS_CHARACTER_INCLUDED
#define YACK_IOS_CHARACTER_INCLUDED 1

#include "yack/memory/small/exclusive.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/data/cxx-pool.hpp"

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
        
        //______________________________________________________________________
        //
        //
        //! list of characters alias
        //
        //______________________________________________________________________
        typedef cxx_list_of<character> characters_;
        
        //______________________________________________________________________
        //
        //
        //! characters
        //
        //______________________________________________________________________
        class characters : public characters_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            characters() throw();                       //!< setup empty
            virtual ~characters() throw();              //!< cleanup
            characters(const characters&);              //!< hard copy
            characters & operator=(const characters &); //!< copy/assign
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void add(const void *addr, size_t size);     //!< add buffer content
            characters & operator<<(const uint8_t);      //!< add single byte
            characters & operator<<(const char  *);      //!< add message
            characters & operator<<(const characters &); //!< copy/merge_back
            
            void    append_front(const uint8_t); //!< push_front
            uint8_t remove_front() throw();      //!< pop_front, return value
            uint8_t remove_back() throw();       //!< pop_back, return value
            
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


