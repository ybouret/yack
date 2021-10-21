

//! \file

#ifndef YACK_HASH64_IBJ_INCLUDED
#define YACK_HASH64_IBJ_INCLUDED 1

#include "yack/hashing/hash64.hpp"

namespace yack
{

    namespace hashing
    {

        //! Bob Jenkin's hash32
        uint32_t ibj32(uint32_t) throw();

        //______________________________________________________________________
        //
        //
        //! hashing 64 bits using Bob Jenkin's hash32
        //
        //______________________________________________________________________
        class ibj64 : public hash64
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const char clid[]; //!< "IBJ"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit ibj64() throw(); //!< setup
            virtual ~ibj64() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual const char *protocol()                                   const throw();
            virtual void        operator()(uint32_t &lword, uint32_t &rword) const throw();

            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ibj64);
        };

    }
}

#endif

