//! \file

#ifndef YACK_EXCEPTION_INCLUDED
#define YACK_EXCEPTION_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/check/printf.hpp"
#include <exception>
#include <iosfwd>

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! base class for an exception, with formattable field
    //
    //__________________________________________________________________________
    class exception : public std::exception
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        static const char   what_text[];       //!< default what()
        static const size_t when_size = 512;   //!< internal buffer size

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~exception()           throw();                 //!< cleanup
        exception()                    throw();                 //!< setup empty
        exception(const exception &)   throw();                 //!< nothrow copy
        exception(const char *fmt,...) throw() YACK_PRINTF_API; //!< setup with format

        //______________________________________________________________________
        //
        // formatting methods
        //______________________________________________________________________
        exception &add(const char *fmt,...)  throw() YACK_PRINTF_API; //!< append a formatted text
        exception &pre(const char *fmt,...)  throw() YACK_PRINTF_API; //!< prepend a formatted text
        exception & operator<<(const char  ) throw();                 //!< append a character
        exception & operator<<(const char *) throw();                 //!< append a text

        virtual const char *what() const throw();          //!< what_text is the default
        const char         *when() const throw();          //!< internal buffer
        void                show(std::ostream &os) const;  //!< cosmetics

        //______________________________________________________________________
        //
        // helpers
        //______________________________________________________________________

        //! format using vsnprintf, args is the address of a va_list
        static void failsafe_format(char *buffer, const size_t buflen, const char *fmt, void *args) throw();

    protected:
        char info[when_size]; //!< internal buffer to store info
        
    private:
        YACK_DISABLE_ASSIGN(exception);
        void clear() throw();
    };

}

#endif
