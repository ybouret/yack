

//! \file

#ifndef YACK_INFO_MODULATION_MTF_INCLUDED
#define YACK_INFO_MODULATION_MTF_INCLUDED 1

#include "yack/info/modulation.hpp"
#include "yack/data/list/raw.hpp"

namespace yack
{

    namespace information
    {
        //______________________________________________________________________
        //
        //
        //! move-to-front codec base class
        //
        //______________________________________________________________________
        class mtf : public modulation
        {
        public:
            virtual ~mtf() noexcept;       //!< cleanup
            virtual void reset() noexcept; //!< reset internal lis

        protected:
            explicit mtf() noexcept; //!< setup

            //! internal node
            struct node_t {
                node_t *next; //!< for list
                node_t *prev; //!< for list
                uint8_t data; //!< current byte
            };

            raw_list_of<node_t> nodes; //!< dyanmic list


        private:
            node_t              store[256];
            YACK_DISABLE_COPY_AND_ASSIGN(mtf);


        };

        //______________________________________________________________________
        //
        //
        //! move-to-front encoder
        //
        //______________________________________________________________________
        class mtf_encoder : public mtf
        {
        public:
            explicit mtf_encoder() noexcept; //!< setup
            virtual ~mtf_encoder() noexcept; //!< cleanup

            virtual uint8_t alter(const uint8_t x) noexcept; //!< mtf

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(mtf_encoder);
        };

        //______________________________________________________________________
        //
        //
        //! move-to-front decoder
        //
        //______________________________________________________________________
        class mtf_decoder : public mtf
        {
        public:
            explicit mtf_decoder() noexcept; //!< setup
            virtual ~mtf_decoder() noexcept; //!< cleanup

            virtual uint8_t alter(const uint8_t x) noexcept; //!< mtf

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(mtf_decoder);
        };


    }
}

#endif
