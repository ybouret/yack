//! \file
#ifndef YACK_HASHING_FUNCTION_INCLUDED
#define YACK_HASHING_FUNCTION_INCLUDED 1


#include "yack/object.hpp"
#include "yack/counted.hpp"
#include "yack/ptr/arc.hpp"

namespace yack
{
    
    namespace memory
    {
        class ro_buffer;
    }

	namespace hashing
    {

        //______________________________________________________________________
        //
        //
        //! base class for hashing function
        //
        //______________________________________________________________________
		class function : public object, public counted
		{
		public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef arc_ptr<function> pointer; //!< pointer type

			const size_t length; //!< output generation
			const size_t window; //!< internal window size

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual            ~function()                             noexcept;     //!< destructor
			virtual const char *name()                           const noexcept = 0; //!< get a name
			virtual void        set()                                  noexcept = 0; //!< initialize
			virtual void        run(const void *buffer, size_t buflen) noexcept = 0; //!< process bytes
			virtual void        get(void *output, size_t outlen)       noexcept = 0; //!< finalize/fill array
            
            //__________________________________________________________________
            //
            // non virtual interface
            //__________________________________________________________________

            //! helper, usually used by the virtual 'get' method.
			static void fill(void *output, size_t outlen, const void *input, size_t inlen) noexcept;
            
            //! run(block_addr,block_size)
            void operator()(const void *block_addr, const size_t block_size) noexcept;
            
            //! run(msg)
            void operator()(const char *msg) noexcept;
            
            //! run(buffer.ro_addr(),buffer.measure());
            void operator()(const memory::ro_buffer &) noexcept;
            
            
            
            //! hash a block
            void        block(void *output, size_t outlen, const void *block_addr, const size_t block_size) noexcept;
            
            //! hash a memory buffer
            void        block(void *output, size_t outlen, const memory::ro_buffer &) noexcept;
            
            //! hash a message
            void        block(void *output, size_t outlen, const char *msg) noexcept;
            
        protected:
            //! initialize function length and window
			explicit function( size_t L, size_t W) noexcept;
			
		private:
			YACK_DISABLE_COPY_AND_ASSIGN(function);
		};
		
        //! format hashing function prototype
#define YACK_HASHING_FUNCTION_DECL(NAME,L,W)                    \
static const char clid[];                                       \
explicit NAME() noexcept;                                        \
virtual ~NAME() noexcept;                                        \
static inline function * create() { return new NAME(); }        \
static const size_t __length = L ;                              \
static const size_t __window = W ;                              \
inline virtual const char *name() const noexcept { return clid;} \
virtual void set() noexcept;                                     \
virtual void run( const void *buf, size_t len ) noexcept;        \
virtual void get( void *output, size_t outlen ) noexcept 

	}

    
}


#endif
