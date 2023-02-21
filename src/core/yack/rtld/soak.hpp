//! \file

#ifndef YACK_SOAK_INCLUDED
#define YACK_SOAK_INCLUDED

#include "yack/rtld/dso.h"
#include "yack/exception.hpp"
#include <cstdio>
#include <cstdlib>

namespace yack
{
	//__________________________________________________________________________
	//
	//! try block...
	//__________________________________________________________________________
#define YACK_SOAK_TRY(FUNC)                  \
/**/  do {                                   \
/**/    const char * const __soak_fn = FUNC; \
/**/    try

	//__________________________________________________________________________
	//
	//! ...to catch'm all
	//___________________________________________________________________________
#define YACK_SOAK_CATCH()                                                       \
/**/    catch(const yack::exception &e)                                      \
/**/    {                                                                    \
/**/      soak::print(stderr,"[%s] %s: %s\n",__soak_fn,e.what(),e.when());   \
/**/    }                                                                    \
/**/    catch(...)                                                           \
/**/    {                                                                    \
/**/      soak::print(stderr,"[%s] %s",__soak_fn,soak::message::unknown);    \
/**/    }                                                                    \
/**/  } while(false)

	//__________________________________________________________________________
	//
	//! code on verbosity
	//__________________________________________________________________________
#define YACK_SOAK_VERBOSE(CODE) do { if(soak::verbose) { CODE; } } while(false)

	//__________________________________________________________________________
	//
	//! success value
	//__________________________________________________________________________
#define YACK_SOAK_SUCCESS ( 0)

	//__________________________________________________________________________
	//
	//! failure value
	//__________________________________________________________________________
#define YACK_SOAK_FAILURE (-1)

	//__________________________________________________________________________
	//
	//
	//! Shared Object Application Kit
	//
	//__________________________________________________________________________
	struct soak
	{
		//______________________________________________________________________
		//
		// definition
		//______________________________________________________________________

		//! internal safe printf
		static void        print(FILE* stream, const char* format, ...) throw() YACK_PRINTF_CHECK(2, 3);
		static bool        verbose; //!< verbose flag

		//! helper for verbose messages
		struct message
		{
			static const char  unknown[];     //!< unknowned exception
			static const char  fmt[];         //!< format to display
			static const char  init[];        //!< "init"
			static const char  quit[];        //!< "quit"
			static const char  call[];        //!< "call"

			//! YACK_SOAK_VERBOSE(soak::print(stderr,fmt,id,msg))
			static void        disp(const char* id, const char* msg) throw();
		};


		//______________________________________________________________________
		//
		//! class to handle ONE instance of an APPLICATION
		/**
		 - the APPLICATION must have a 'call_sign'
		 - the APPLICATION must have a default constructor,
		 and the arguments may be set by some static members
		 - once the DLL is loaded, the user can setup some global
		 communication variables, then call Init(). When done,
		 the user should call Quit()
		 */
		 //______________________________________________________________________
		template <typename APPLICATION>
		class app
		{
		public:
			//! cleanup
			inline virtual           ~app() throw() {}

			//! soname from call_sign
			static inline const char* soname() throw() { return APPLICATION::call_sign; }

			//! get initialized (by derived class) instance
			static inline APPLICATION& _() throw() {
				if (!was_init())
				{
					soak::print(stderr, "%s was not initialized!!\n", APPLICATION::call_sign);
					exit(EXIT_FAILURE);
				}
				return *(APPLICATION*)instance;
			}

			//! return status
			static inline int was_init() throw()
			{
				return (NULL != instance) ? 1 : 0;
			}


		protected:
			//! constructor
			inline explicit app() throw() {}

			//! cleanup
			inline static void quit() throw()
			{
				message::disp(soname(), message::quit);
				if (instance)
				{
					delete instance;
					instance = 0;
				}
			}

			//! create the instance
			/**
			 use static members for initialization
			 */
			inline static APPLICATION* init() throw()
			{
				assert(!was_init() || die("alread initialized"));
				YACK_SOAK_TRY(soname())
				{
					message::disp(soname(), message::init);
					instance = (volatile APPLICATION*) new APPLICATION();
					return (APPLICATION*)instance;
				}
				YACK_SOAK_CATCH();
				return NULL;
			}

		private:
			YACK_DISABLE_COPY_AND_ASSIGN(app);
			static volatile APPLICATION* instance;
		};


	};

	template <typename T>  volatile T* soak::app<T>::instance = 0;

	//__________________________________________________________________________
	//
	//
	//! common code to initialize CLASS Init/Quit an call_sign
	//
	//__________________________________________________________________________
#define YACK_SOAK_INIT_(CLASS) \
/**/  {                                                                                   \
/**/    public:                                                                           \
/**/      inline static void   Quit()    throw() { soak::app<CLASS>::quit();            } \
/**/      inline static CLASS *Init()    throw() { return soak::app<CLASS>::init();     } \
/**/      inline static int    WasInit() throw() { return soak::app<CLASS>::was_init(); } \
/**/      static const char call_sign[]

	//! no parameters macro
#define YACK_SOAK_NO_PARAMS void

	//! no code macro
#define YACK_SOAK_NO_LOADER (void)0

	//__________________________________________________________________________
	//
	//! finalize the code
	/**
	 - export a CLASSQuit function to delete instance
	 - export a CLASSInit(PARAMS) function, performing LOADER code before
	 calling the internal CLASS::Init()
	 - export a CLASSWasInit() function to check status
	 - implement the call_sign for the CLASS
	 */
	 //__________________________________________________________________________
#define YACK_SOAK_FINISH(CLASS,PARAMS,LOADER)                               \
/**/    private:                                                         \
/**/      Y_DISABLE_COPY_AND_ASSIGN(CLASS);                              \
/**/      inline virtual ~CLASS() throw() {}                             \
/**/      friend class soak::app<CLASS>;                                 \
/**/  };                                                                 \
/**/ YACK_DLL_EXTERN()                                                      \
/**/ YACK_EXPORT void YACK_DLL_API CLASS##Quit() throw() { CLASS::Quit(); }    \
/**/ YACK_EXPORT int  YACK_DLL_API CLASS##Init(PARAMS) throw() {               \
/**/   do { LOADER; } while(false);                                      \
/**/   return (NULL != CLASS::Init()) ? YACK_SOAK_SUCCESS : YACK_SOAK_FAILURE; \
/**/ }                                                                   \
/**/ YACK_EXPORT int YACK_DLL_API CLASS##WasInit() throw()                     \
/**/   { return CLASS::WasInit(); }                                      \
/**/ YACK_DLL_FINISH()                                                      \
/**/ const char CLASS::call_sign[] = #CLASS

	//__________________________________________________________________________
	//
	//! direct finish class
	//__________________________________________________________________________
#define YACK_SOAK_POLISH(CLASS) YACK_SOAK_FINISH(CLASS,YACK_SOAK_NO_PARAMS,YACK_SOAK_NO_LOADER)

	//__________________________________________________________________________
	//
	//! common CLASS declaration
	//__________________________________________________________________________
#define YACK_SOAK_DECL_(CLASS) class CLASS : public soak::app<CLASS>


	//__________________________________________________________________________
	//
	//! declare simple class
	//__________________________________________________________________________
#define YACK_SOAK_DECLARE(CLASS)  \
/**/    YACK_SOAK_DECL_(CLASS)    \
/**/    YACK_SOAK_INIT_(CLASS)

	//__________________________________________________________________________
	//
	//! declare derived class
	//__________________________________________________________________________
#define YACK_SOAK_DERIVED(CLASS,BASE)       \
/**/    YACK_SOAK_DECL_(CLASS), public BASE \
/**/    YACK_SOAK_INIT_(CLASS)


	//__________________________________________________________________________
	//
	//! declare exported public function
	//__________________________________________________________________________
#define YACK_SOAK_PUBLIC(RETURN_TYPE,PROTO) \
YACK_DLL_EXTERN()                           \
YACK_EXPORT RETURN_TYPE YACK_DLL_API PROTO

	//__________________________________________________________________________
	//
	//! finish exported function
	//__________________________________________________________________________
#define YACK_SOAK_RETURN()  YACK_DLL_FINISH()


}

#endif
