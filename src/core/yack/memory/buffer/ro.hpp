
//! \file

#ifndef YACK_MEMORY_BUFFER_RO_INCLUDED
#define YACK_MEMORY_BUFFER_RO_INCLUDED 1

#include "yack/setup.hpp"
#include <iosfwd>

namespace yack
{

    namespace memory
    {

        //______________________________________________________________________
        //
        //
        //! read-only buffer interface
        //
        //______________________________________________________________________
        class ro_buffer
        {
        public:
            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual size_t      measure() const throw() = 0; //!< usable bytes
            virtual const void *ro_addr() const throw() = 0; //!< first byte address

            //__________________________________________________________________
            //
            // helper
            //__________________________________________________________________
            std::ostream &display_hexa(std::ostream &)             const;         //!< hexadecimal content
            bool          has_same_content_than(const ro_buffer &) const throw(); //!< check

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~ro_buffer() throw(); //!< cleanup
        protected:
            explicit ro_buffer() throw(); //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ro_buffer);
        };

    }

}

#endif
