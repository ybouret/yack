//! \file

#ifndef YACK_IOS_CHARACTER_INCLUDED
#define YACK_IOS_CHARACTER_INCLUDED 1

#include "yack/memory/small/exclusive.hpp"
#include "yack/data/list/concrete.hpp"
#include "yack/data/pool/concrete.hpp"

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
        //! base class for characters
        //
        //______________________________________________________________________
        typedef klist<uint8_t,character> characters_;

        //______________________________________________________________________
        //
        //
        //! pool for local cache
        //
        //______________________________________________________________________
        typedef kpool<uint8_t,character> repository;

        //______________________________________________________________________
        //
        //
        //! concrete list of characters
        //
        //______________________________________________________________________
        class characters : public characters_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            characters()          throw();              //!< setup empty
            virtual ~characters() throw();              //!< cleanup
            characters(const characters &);             //!< copy
            characters & operator=(const characters &); //!< assign by copy swap
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void         load(const void *, size_t);    //!< add a block
            characters & operator<<(const char   *msg); //!< add a message
            characters & operator<<(const uint8_t chr); //!< add a byte

            //! display as C-style string
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const characters &chars)
            {
                for(const node_type *node = chars.head;node;node=node->next)
                {
                    os << **node;
                }
                return os;
            }

        };



       
    
        
    }
}

#endif


