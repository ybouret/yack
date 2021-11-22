
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
            explicit string_(size_t       num,
                             const size_t bpc);

            virtual const void * ro_addr() const throw();
            virtual size_t       measure() const throw();

        protected:
            size_t       chars; //!< [0..width]
        public:                 //|
            const size_t width; //!< max items
            const size_t space; //!< width+1
        private:                //|
            void        *block; //!< address of first char
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(string_);
        };



    }

}

#endif

