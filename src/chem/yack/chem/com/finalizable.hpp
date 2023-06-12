//! \file

#ifndef YACK_FINALIZABLE_INCLUDED
#define YACK_FINALIZABLE_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace Chemical
    {
        //! helper class
        class Finalizable
        {
        public:
            explicit Finalizable() noexcept; //!< setup
            virtual ~Finalizable() noexcept; //!< cleanup

            //! check doFinalize and update status
            void finalize(const char * const clid);
            
            const bool finalized; //!< status flag


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Finalizable);
            virtual void doFinalize() = 0;
        };
    }
}

#endif

