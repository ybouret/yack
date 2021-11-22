
//! \file

#ifndef YACK_KERNEL_STRING__INCLUDED
#define YACK_KERNEL_STRING__INCLUDED 1

#include "yack/object.hpp"
#include "yack/counted.hpp"
#include "yack/memory/buffer/ro.hpp"

namespace yack
{

    namespace kernel
    {

        class string_ : public object, public counted, public memory::ro_buffer
        {
        public:
            virtual ~string_() throw();


            virtual const void * ro_addr() const throw();

        protected:
            explicit string_(size_t       num,
                             const size_t bpc);
            explicit string_(const string_ &str,
                             const size_t   bpc);
            
            size_t       chars; //!< [0..width]
        public:                 //|
            const size_t width; //!< max items
            const size_t space; //!< width+1
        protected:              //|
            void        *block; //!< address of first char

            void swap_with(string_ &other) throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(string_);
        };



    }

}

#endif

