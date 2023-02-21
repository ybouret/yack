//! \file

#ifndef YACK_INFORMATION_ENTROPY_INCLUDED
#define YACK_INFORMATION_ENTROPY_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace information
    {

        //______________________________________________________________________
        //
        //
        //! entropy computation
        //
        //______________________________________________________________________
        class entropy
        {
        public:

            explicit entropy() noexcept; //!< setup
            virtual ~entropy() noexcept; //!< cleanup

            void      reset()                          noexcept;  //!< restart
            entropy & operator<<(const uint8_t)        noexcept; //!< add one byte
            entropy & operator()(const void * ,size_t) noexcept; //!< add one block
            double    operator()(void)           const noexcept; //!< compute

            //! entropy of a block
            static double of(const void *, const size_t) noexcept;

        private:
            size_t total;
            size_t count[256];

            YACK_DISABLE_COPY_AND_ASSIGN(entropy);
        };

    }

}

#endif
