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

            explicit entropy() throw(); //!< setup
            virtual ~entropy() throw(); //!< cleanup

            void      reset()                          throw();  //!< restart
            entropy & operator<<(const uint8_t)        throw(); //!< add one byte
            entropy & operator()(const void * ,size_t) throw(); //!< add one block
            double    operator()(void)           const throw(); //!< compute

            //! entropy of a block
            static double of(const void *, const size_t) throw();

        private:
            size_t total;
            size_t count[256];

            YACK_DISABLE_COPY_AND_ASSIGN(entropy);
        };

    }

}

#endif
