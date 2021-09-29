
//! \file

#ifndef YACK_SYSTEM_EXCEPTION_INCLUDED
#define YACK_SYSTEM_EXCEPTION_INCLUDED 1

#include "yack/exception.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! base class to fetch an internal textual error message
    //
    //__________________________________________________________________________
    class system_exception : public exception
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
        virtual ~system_exception()                throw(); //!< cleanup
        system_exception(const system_exception &) throw(); //!< nothrow copy

        //______________________________________________________________________
        //
        // interface
        //______________________________________________________________________
        virtual const char *what() const throw(); //!< return system error text

    protected:
        explicit system_exception() throw(); //!< setup
        char text[what_size];                //!< textual error

    private:
        YACK_DISABLE_ASSIGN(system_exception);
        void erase() throw();
    };

    //__________________________________________________________________________
    //
    //
    //! generic system exception with an integral error code
    //
    //__________________________________________________________________________
    template <typename CODE>
    class sys_exception : public system_exception
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef sys_exception<CODE> excp_type; //!< the base class

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const CODE code; //!< the system error code

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~sys_exception() throw() { (CODE&)code=0; }

    protected:
        //! setup code/empty what() and when()
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
        //______________________________________________________________________
        //
        //
        //! exception to use errno
        //
        //______________________________________________________________________
        class exception : public sys_exception<int>
        {
        public:
            //! setup code/strerror/when
            explicit exception(const int err, const char *fmt,...) throw() YACK_PRINTF_CHECK(3,4);
            virtual ~exception() throw();        //!< cleanup
            exception(const exception&) throw(); //!< nothrow copy

        private:
            YACK_DISABLE_ASSIGN(exception);
        };
    }

#if defined(YACK_WIN)
	namespace win32
	{
        //______________________________________________________________________
        //
        //
        //! exception to use GetLastError
        //
        //______________________________________________________________________
		class exception : public sys_exception<uint32_t>
		{
		public:
            //! setup code/FormatMessage/when
			explicit exception(const uint32_t err, const char *fmt, ...) throw() YACK_PRINTF_CHECK(3, 4);
			virtual ~exception() throw();        //!< cleanup
			exception(const exception&) throw(); //!< nothrow copy

		private:
			YACK_DISABLE_ASSIGN(exception);
		};
	}
#endif

}

#endif
