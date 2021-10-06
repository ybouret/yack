//! \file
#ifndef YACK_SINGLETON_INCLUDED
#define YACK_SINGLETON_INCLUDED 1

#include "yack/concurrent/mutex.hpp"
#include "yack/arith/align.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/type/destruct.hpp"
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
            mutex                    access; //!< to protect access

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

    //__________________________________________________________________________
    //
    //
    //! singleton base class
    //
    //__________________________________________________________________________
    template <class CLASS>
    class singleton : public concurrent::singleton
    {
    public:
        //______________________________________________________________________
        //
        //! query existence
        //______________________________________________________________________
        static inline bool exists() throw() { return NULL!=instance_; }
        
        //______________________________________________________________________
        //
        //! create/recall instance, with double locking pattern
        //______________________________________________________________________
        static inline CLASS & instance()
        {
            // local satic variables
            static volatile void *impl[ YACK_WORDS_FOR(CLASS) ] = { 0 };
            static volatile bool  init = true;

            // check/create
            YACK_GIANT_LOCK();
            if(!instance_)
            {
                YACK_GIANT_LOCK();
                if(init)
                {
                    // called only once
                    at_exit::perform(destroy,0,CLASS::life_time);
                    init = false;
                }
                new (out_of_reach::zset(impl,sizeof(impl))) CLASS();
                return *(instance_ = coerce_cast<CLASS>(impl));
            }
            else
                return *instance_;
        }
        
      
        //______________________________________________________________________
        //
        //! recall a valid instance
        //______________________________________________________________________
        static CLASS & location() throw() { assert(exists()); return *instance_; }
        
        
        
    protected:
        //! setup
        inline explicit singleton() throw() : concurrent::singleton(CLASS::call_sign,CLASS::life_time)
        {
            if(verbose) enter();
        }
        
        //! cleanup
        inline virtual ~singleton() throw()
        {
            if(verbose) leave();
        }
        
        //______________________________________________________________________
        //
        //! suppress function, may be called by derived class
        //______________________________________________________________________
        static inline void suppress_() throw()
        {
            YACK_GIANT_LOCK();
            if(instance_) {
                YACK_GIANT_LOCK();
                destroy(NULL);
            }
        }
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(singleton);
        static inline void destroy(void*) throw()
        {
            if(instance_) {
                instance_->~CLASS();
                out_of_reach::zset(instance_,sizeof(CLASS));
                instance_ = NULL;
            }
        }
        
        static CLASS *instance_;
        
    };
    
    template <class CLASS> CLASS *            singleton<CLASS>::instance_ = NULL;

}

#endif

