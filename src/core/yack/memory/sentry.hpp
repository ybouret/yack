//! \file

#ifndef YACK_MEMORY_SENTRY_INCLUDED
#define YACK_MEMORY_SENTRY_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/type/transmogrify.hpp"

namespace yack
{
    namespace memory
    {

        //______________________________________________________________________
        //
        //
        //! sentry to detect memory access
        //
        //______________________________________________________________________
        class sentry
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
             typedef uint64_t key_t; //!< integral hash key

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! store information and hash block
            sentry(const char  *file,
                   const int    line,
                   const void  *block_addr,
                   const size_t block_size) throw();

            //! store information and hash INTEGRAL type
            template <typename T> inline
            sentry(const char *file,
                   const int   line,
                   const T    &host) throw() :
            wksp()
            {
                setup(file,line,&host,sizeof(host));
            }

            //! store info for array of items
            template <typename T> inline
            sentry(const char  *file,
                   const int    line,
                   T           *entry,
                   const size_t items,
                   const transmogrify_t &) throw()
            {
                assert(yack_good(entry,items));
                setup(file,line,entry,items*sizeof(T));
            }



            //! access key
            key_t operator*() const throw();

            //! recompute key, exit on error
            ~sentry() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(sentry);
            mutable void *wksp[256/sizeof(void*)];
            void  setup(const char  *file,
                        const int    line,
                        const void  *block_addr,
                        const size_t block_size) throw();
        };

    }
}

//! create the sentry name
#define YACK_MEM_SENTRY__(X,Y) X##Y

//! instantiate the sentry name
#define YACK_MEM_SENTRY_(ID,BLOCK_ADDR,BLOCK_SIZE) volatile yack::memory::sentry  YACK_MEM_SENTRY__(__sentry,ID)(__FILE__,ID,BLOCK_ADDR,BLOCK_SIZE)

//! instantiate the sentry name, template
#define YACK_MEM_SENTRY_T(ID,ENTRY,ITEMS) volatile yack::memory::sentry  YACK_MEM_SENTRY__(__sentry,ID)(__FILE__,ID,ENTRY,ITEMS,yack::transmogrify)

//! instantiate the sentry name
#define YACK_MEM_SENTRY_ON_(ID,HOST)               volatile yack::memory::sentry  YACK_MEM_SENTRY__(__sentry,ID)(__FILE__,ID,HOST)

//! use a local sentry on block_addr/block_size
#define YACK_MEM_SENTRY(BLOCK_ADDR,BLOCK_SIZE) YACK_MEM_SENTRY_(__LINE__,BLOCK_ADDR,BLOCK_SIZE)

//! use a local sentry on an integral host
#define YACK_MEM_SENTRY_ON(HOST)               YACK_MEM_SENTRY_ON_(__LINE__,HOST)

//! use a local sentry on an array of items
#define YACK_MEM_SENTRY_FOR(ENTRY,ITEMS)       YACK_MEM_SENTRY_T(__LINE__,ENTRY,ITEMS)


#endif

