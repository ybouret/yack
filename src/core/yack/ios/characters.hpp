
//! \file

#ifndef YACK_IOS_CHARACTERS_INCLUDED
#define YACK_IOS_CHARACTERS_INCLUDED 1

#include "yack/ios/character.hpp"

namespace yack
{
    namespace ios
    {
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

            char        *cstr() const;         //!< filled from legacy::acquire
            static void  free(char *) throw(); //!< free   with legacy::release

        };

    }

}

#endif

