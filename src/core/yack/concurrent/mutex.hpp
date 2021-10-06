
//! \file

#ifndef YACK_SYNC_MUTEX_INCLUDED
#define YACK_SYNC_MUTEX_INCLUDED 1

#include "yack/lockable.hpp"
#include "yack/concurrent/primitive.hpp"

namespace yack
{
    namespace concurrent
    {
        //______________________________________________________________________
        //
        //
        // forward declarations
        //
        //______________________________________________________________________
        class condition;
        namespace quark { struct mutex;}
        
        //______________________________________________________________________
        //
        //
        //! recursive mutex
        /**
         The system's mutex is hold within the quark::mutex.
         The methods are directly managed from within the internal atelier.
         */
        //
        //______________________________________________________________________
        class mutex : public primitive, public lockable
        {
        public:
            static bool verbose; //!< flag to trace calls

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit     mutex(const char *id=0); //!< create internal mutex
            virtual     ~mutex() throw();         //!< cleanup

            //__________________________________________________________________
            //
            // lockable interface
            //__________________________________________________________________
            virtual void lock()     throw(); //!< lock
            virtual void unlock()   throw(); //!< unlock
            virtual bool try_lock() throw(); //!< try lock

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void tag(const char *id) throw(); //!< create a name

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(mutex);
            quark::mutex *impl;
            friend class condition;
        public:
            const char    name[64-3*sizeof(void*)]; //!< identifier to trace
        };
    }
}

#endif

