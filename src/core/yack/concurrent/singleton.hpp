
//! \file
#ifndef YACK_CONCURRENT_SINGLETON_INCLUDED
#define YACK_CONCURRENT_SINGLETON_INCLUDED 1

#include "yack/concurrent/mutex.hpp"
#include "yack/system/at-exit.hpp"
#include <new>

namespace yack
{

    namespace concurrent
    {
        //______________________________________________________________________
        //
        //
        //! singleton base class with information an access
        //
        //______________________________________________________________________
        class singleton
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static bool   verbose; //!< verbosity to trace management

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const char * const       uuid;   //!< call_sign
            const at_exit::longevity span;   //!< life_time
            const size_t             _len;   //!< strlen(uuid)
            mutable mutex            access; //!< to protect access

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~singleton() throw(); //!< cleanup
        protected:
            //! setup from call_sign and life_time
            explicit singleton(const char *             call_sign,
                               const at_exit::longevity life_time) throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void     enter() const throw(); //!< show creation
            void     leave() const throw(); //!< show destruction

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(singleton);
            void     show() const;
        };
    }

}

#endif

