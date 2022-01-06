
//! \file

#ifndef YACK_SYSTEM_IMPORTED_INCLUDED
#define YACK_SYSTEM_IMPORTED_INCLUDED 1

#include "yack/exception.hpp"

namespace yack
{

    namespace imported
    {
        //__________________________________________________________________________
        //
        //
        //! base class to fetch an internal textual error message
        //
        //__________________________________________________________________________
        class exception : public yack::exception
        {
        public:
            //______________________________________________________________________
            //
            // types and definitions
            //______________________________________________________________________
            static const size_t what_size = 128; //!< internal buffer size

            //______________________________________________________________________
            //
            // C++
            //______________________________________________________________________
            explicit exception()         throw(); //!< empty
            virtual ~exception()         throw(); //!< cleanup
            exception(const exception &) throw(); //!< nothrow copy
            //! set what()=where, format when()
            explicit exception(const char *where, const char *fmt,...) throw() YACK_PRINTF_CHECK(3,4);
            
            //______________________________________________________________________
            //
            // interface
            //______________________________________________________________________
            virtual const char *what() const throw(); //!< return system error text

            //______________________________________________________________________
            //
            //! helper to format text
            //______________________________________________________________________
            void describe(const char *fmt,...) throw() YACK_PRINTF_API;

        protected:
            char text[what_size];                //!< textual error

        private:
            YACK_DISABLE_ASSIGN(exception);
            void erase() throw();
        };
    }

}

#endif
