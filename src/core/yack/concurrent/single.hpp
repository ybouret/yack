
//! \file
#ifndef YACK_CONCURRENT_SINGLE_INCLUDED
#define YACK_CONCURRENT_SINGLE_INCLUDED 1

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
        //! single  base class with information an access
        //
        //______________________________________________________________________
        class single
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
            virtual ~single() noexcept; //!< cleanup

        protected:
            //! setup from call_sign and life_time
            explicit single(const char *             call_sign,
                            const at_exit::longevity life_time) noexcept;

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void        enter() const noexcept; //!< show creation
            void        leave() const noexcept; //!< show destruction
            //! display info if verbose
            static void shrug(const char              *call_sign,
                              const at_exit::longevity life_time);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(single);
            void     show() const;
        };
    }

}

#endif

