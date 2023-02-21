//! \file

#ifndef YACK_INFO_MODULATION_DELTA_INCLUDED
#define YACK_INFO_MODULATION_DELTA_INCLUDED 1

#include "yack/info/modulation.hpp"

namespace yack
{

    namespace information
    {

        //______________________________________________________________________
        //
        //
        //! delta codec base class
        //
        //______________________________________________________________________
        class delta : public modulation
        {
        public:
            virtual     ~delta() noexcept; //!< cleanup
            virtual void reset() noexcept; //!< last=0

        protected:
            explicit delta() noexcept; //!< setup
            uint8_t  last;            //!< pivot value

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(delta);


        };

        //______________________________________________________________________
        //
        //
        //! delta encoder
        //
        //______________________________________________________________________
        class delta_encoder : public delta
        {
        public:
            explicit delta_encoder() noexcept; //!< setup
            virtual ~delta_encoder() noexcept; //!< cleanup

            virtual uint8_t alter(const uint8_t x) noexcept; //!< delta

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(delta_encoder);
        };

        //______________________________________________________________________
        //
        //
        //! delta decoder
        //
        //______________________________________________________________________
        class delta_decoder : public delta
        {
        public:
            explicit delta_decoder() noexcept; //!< setup
            virtual ~delta_decoder() noexcept; //!< cleanup

            virtual uint8_t alter(const uint8_t x) noexcept; //!< delta

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(delta_decoder);
        };


    }
}

#endif
