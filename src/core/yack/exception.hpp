//! \file

#ifndef YACK_EXCEPTION_INCLUDED
#define YACK_EXCEPTION_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/check/printf.hpp"
#include <exception>
#include <iosfwd>

namespace yack
{

    class exception : public std::exception
    {
    public:
        static const char   what_text[];
        static const size_t when_size = 512;

        virtual ~exception() throw();

        exception()                  throw();
        exception(const exception &) throw();
        exception(const char *fmt,...) throw() YACK_PRINTF_API;

        exception &add(const char *fmt,...) throw() YACK_PRINTF_API;
        exception &pre(const char *fmt,...) throw() YACK_PRINTF_API;

        exception & operator<<(const char  ) throw();
        exception & operator<<(const char *) throw();

        virtual const char *what() const throw();   //!< what_text
        const char         *when() const throw();   //!< internal buffer
        void                show(std::ostream &os) const;


        

    private:
        YACK_DISABLE_ASSIGN(exception);
        void clear() throw();
        char info[when_size];
    };

}

#endif
