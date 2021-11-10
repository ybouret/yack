//! \file
#ifndef YACK_HASHING_FUNCTION_INCLUDED
#define YACK_HASHING_FUNCTION_INCLUDED 1


#include "yack/setup.hpp"

namespace yack
{
    
	namespace hashing
	{
		
        //! base class for hashing function
		class function //: public counted_object
		{
		public:
            //typedef arc_ptr<function> pointer; //!< pointer type

			const size_t length; //!< output generation
			const size_t window; //!< internal window size

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual            ~function()     throw();     //!< destructor
			virtual const char *name()   const throw() = 0; //!< get a name
			virtual void        set() throw() = 0;          //!< initialize
			virtual void        run(const void *buffer, size_t buflen) throw() = 0; //!< process bytes
			virtual void        get(void *output, size_t outlen) throw() = 0;       //!< finalize/fill array

            
            
            //! helper, usually used by the virtual 'get' method.
			static void fill(void *output, size_t outlen, const void *input, size_t inlen) throw();
            
		protected:
            //! initialize function length and window
			explicit function( size_t L, size_t W) throw();
			
		private:
			YACK_DISABLE_COPY_AND_ASSIGN(function);
		};
		
        //! format hashing function prototype
#define YACK_HASHING_FUNCTION_DECL(NAME,L,W)                    \
static const char clid[];                                       \
explicit NAME() throw();                                        \
virtual ~NAME() throw();                                        \
static inline function * create() { return new NAME(); }        \
static const size_t __length = L ;                              \
static const size_t __window = W ;                              \
inline virtual const char *name() const throw() { return clid;} \
virtual void set() throw();                                     \
virtual void run( const void *buf, size_t len ) throw();        \
virtual void get( void *output, size_t outlen ) throw() 

	}

    
}


#endif
