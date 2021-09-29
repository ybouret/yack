
//! \file

#ifndef YACK_SYSTEM_EXCEPTION_INCLUDED
#define YACK_SYSTEM_EXCEPTION_INCLUDED 1

#include "yack/exception.hpp"

namespace yack
{

    class system_exception : public exception
    {
    public:
        static const size_t what_size = 128;

        virtual ~system_exception() throw();
        system_exception(const system_exception &other) throw();
        virtual const char *what() const throw();

    protected:
        explicit system_exception() throw();
        char text[what_size];

    private:
        YACK_DISABLE_ASSIGN(system_exception);
        void erase() throw();
    };

    template <typename CODE>
    class sys_exception : public system_exception
    {
    public:
        typedef sys_exception<CODE> excp_type;
        
        inline virtual ~sys_exception() throw() { (CODE&)code=0; }
        const CODE code;

    protected:
        inline explicit sys_exception(const CODE err) throw() :
        system_exception(),
        code(err)
        {
        }

    private:
        YACK_DISABLE_ASSIGN(sys_exception);

    };

    namespace libc
    {
        class exception : public sys_exception<int>
        {
        public:
            explicit exception(const int err, const char *fmt,...) throw() YACK_PRINTF_CHECK(3,4);
            virtual ~exception() throw();
            exception(const exception&) throw();

        private:
            YACK_DISABLE_ASSIGN(exception);
        };
    }



}

#endif
