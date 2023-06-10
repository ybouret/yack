//! \file

#ifndef YACK_FINALIZABLE_INCLUDED
#define YACK_FINALIZABLE_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace Chemical
    {
        class Finalizable
        {
        public:
            explicit Finalizable() noexcept;
            virtual ~Finalizable() noexcept;

            void finalize(const char * const clid);
            
            const bool finalized;


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Finalizable);
            virtual void doFinalize() = 0;
        };
    }
}

#endif

