
//! \file

#ifndef YACK_HASH64_DES_INCLUDED
#define YACK_HASH64_DES_INCLUDED 1

#include "yack/hashing/hash64.hpp"

namespace yack
{

    namespace hashing
    {

        //______________________________________________________________________
        //
        //
        //! using DES to hash 64 bits
        //
        //______________________________________________________________________
        class des64 : public hash64
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const char clid[]; //!< "DES"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit des64() noexcept; //!< setup
            virtual ~des64() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // hash64 interface
            //__________________________________________________________________
            virtual const char *protocol()                                   const noexcept; //!< clid
            virtual void        operator()(uint32_t &lword, uint32_t &rword) const noexcept; //!< algo

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            bool test() const noexcept; //!< test consistency

            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(des64);
        };

    }
}

#endif

