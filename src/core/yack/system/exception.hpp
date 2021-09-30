
//! \file

#ifndef YACK_SYSTEM_EXCEPTION_INCLUDED
#define YACK_SYSTEM_EXCEPTION_INCLUDED 1

#include "yack/system/imported.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! generic system exception with an integral error code
    //
    //__________________________________________________________________________
    template <typename CODE>
    class system_exception : public imported::exception
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef system_exception<CODE> excp_type; //!< the base class

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const CODE code; //!< the system error code

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! no-throw copy
        inline system_exception(const system_exception &other) throw() :
        imported::exception(other), code(other.code)  {}

        //! cleanup
        inline virtual ~system_exception() throw() { (CODE&)code=0; }

    protected:
        //! setup code/empty what() and when()
        inline explicit system_exception(const CODE err) throw() :
        imported::exception(), code(err) {}
        
    private:
        YACK_DISABLE_ASSIGN(system_exception);

    };

    namespace libc
    {
        //______________________________________________________________________
        //
        //
        //! exception to use errno
        //
        //______________________________________________________________________
        class exception : public system_exception<int>
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
		class exception : public system_exception<uint32_t>
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
