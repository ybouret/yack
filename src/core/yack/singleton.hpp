//! \file
#ifndef YACK_SINGLETON_INCLUDED
#define YACK_SINGLETON_INCLUDED 1

#include "yack/concurrent/single.hpp"
#include "yack/arith/align.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/type/destruct.hpp"
#include <new>

namespace yack
{
    
    //__________________________________________________________________________
    //
    //
    //! singleton base class
    //
    //__________________________________________________________________________
    template <class CLASS>
    class singleton : public concurrent::single
    {
    public:
        //______________________________________________________________________
        //
        //
        //! query existence
        //
        //______________________________________________________________________
        static inline bool exists() throw() { return NULL!=instance_; }
        
        //______________________________________________________________________
        //
        //
        //! create/recall instance, with double locking pattern
        //
        //______________________________________________________________________
        static inline CLASS & instance()
        {
            //__________________________________________________________________
            //
            // local satic variables
            //__________________________________________________________________
            static volatile void *impl[ YACK_WORDS_FOR(CLASS) ] = { 0 };
            static volatile bool  init = true;

            //__________________________________________________________________
            //
            // check/create
            //__________________________________________________________________
            YACK_GIANT_LOCK();
            if(!instance_)
            {
                YACK_GIANT_LOCK();
                if(init)
                {
                    // called only once
                    shrug(CLASS::call_sign,CLASS::life_time);
                    at_exit::perform(destroy,0,CLASS::life_time);
                    init = false;
                }
                new ( YACK_STATIC_ZSET(impl) ) CLASS();
                return *(instance_ = coerce_cast<CLASS>(impl));
            }
            else
                return *instance_;
        }
        
      
        //______________________________________________________________________
        //
        //
        //! recall a valid instance
        //
        //______________________________________________________________________
        static CLASS & location() throw() { assert(exists()); return *instance_; }
        
        
        
    protected:
        //! setup
        inline explicit singleton() throw() : concurrent::single(CLASS::call_sign,CLASS::life_time)
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

