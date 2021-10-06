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

    namespace kernel
    {
        class singleton
        {
        public:
            static bool verbose;

            virtual ~singleton() throw();


        protected:
            explicit singleton() throw();
            static void enter(const char *call_sign, const at_exit::longevity life_time) throw();
            static void leave(const char *call_sign, const at_exit::longevity life_time) throw();



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(singleton);
        };
    }

    //__________________________________________________________________________
    //
    //
    //! singleton base class
    //
    //__________________________________________________________________________
    template <class CLASS>
    class singleton : public kernel::singleton
    {
    public:
        //______________________________________________________________________
        //
        //! unique access
        //______________________________________________________________________
        static concurrent::mutex access;
        
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
            YACK_LOCK(access);
            if(!instance_)
            {
                YACK_LOCK(access);
                if(verbose) enter(CLASS::call_sign,CLASS::life_time);
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
        inline explicit singleton() throw() {}
        
        //! cleanup
        inline virtual ~singleton() throw() {}
        
        //______________________________________________________________________
        //
        //! suppress function, may be called by derived class
        //______________________________________________________________________
        static inline void suppress_() throw()
        {
            YACK_LOCK(access);
            if(instance_) {
                YACK_LOCK(access);
                destroy(NULL);
            }
        }
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(singleton);
        static inline void destroy(void*) throw()
        {
            if(instance_) {
                if(verbose) leave(CLASS::call_sign,CLASS::life_time);
                instance_->~CLASS();
                out_of_reach::zset(instance_,sizeof(CLASS));
                instance_ = NULL;
            }
        }
        
        static CLASS *instance_;
        
    };
    
    template <class CLASS> CLASS *            singleton<CLASS>::instance_ = NULL;
    template <class CLASS> concurrent::mutex  singleton<CLASS>::access;

}

#endif

