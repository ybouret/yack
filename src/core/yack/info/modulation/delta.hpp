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
            virtual     ~delta() throw(); //!< cleanup
            virtual void reset() throw(); //!< last=0

        protected:
            explicit delta() throw(); //!< setup
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
            explicit delta_encoder() throw(); //!< setup
            virtual ~delta_encoder() throw(); //!< cleanup

            virtual uint8_t alter(const uint8_t x) throw(); //!< delta

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
            explicit delta_decoder() throw(); //!< setup
            virtual ~delta_decoder() throw(); //!< cleanup

            virtual uint8_t alter(const uint8_t x) throw(); //!< delta

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(delta_decoder);
        };


    }
}

#endif
