
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
        //______________________________________________________________________
        //
        //
        //! memory handling for strings
        //
        //______________________________________________________________________
        class string_ : public object, public counted, public memory::ro_buffer
        {
        public:
            //! memory buffer interface
            virtual const void * ro_addr() const throw();

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~string_() throw();            //!< cleanup
        protected:                                 //|
            explicit string_(size_t         num,   //|
                             const size_t   bpc);  //!< setup for width>=num
            explicit string_(const string_ &str,   //|
                             const size_t   bpc);  //!< copy

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            size_t       chars; //!< [0..width]
        public:                 //|
            const size_t width; //!< max items
            const size_t space; //!< width+1
        protected:              //|
            void        *block; //!< address of first char

            void swap_with(string_ &other) throw(); //!< no-throw swap

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(string_);
        };



    }

}

#endif

