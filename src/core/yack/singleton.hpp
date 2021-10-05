//! \file
#ifndef YACK_SINGLETON_INCLUDED
#define YACK_SINGLETON_INCLUDED 1

#include "yack/concurrent/mutex.hpp"
#include "yack/arith/align.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/type/destruct.hpp"
#include "yack/system/at-exit.hpp"

namespace yack
{
    
    template <class CLASS>
    class singleton
    {
    public:
        static concurrent::mutex access;
        
        static inline bool exists() throw()
        {
            return NULL!=instance_;
        }
        
        static CLASS & instance()
        {
            static volatile void *impl[ YACK_WORDS_FOR(CLASS) ] = { 0 };
            static volatile bool  init = true;
            
            YACK_LOCK(access);
            if(!instance_)
            {
                YACK_LOCK(access);
                out_of_reach::zset(impl,sizeof(impl));
                if(init)
                {
                    at_exit::perform(destroy,0,CLASS::life_time);
                    init = false;
                }
                new (impl) CLASS();
                instance_ = coerce_cast<CLASS>(impl);
            }
            assert(instance_);
            return *instance_;
        }
        
      
        
        static CLASS & location() throw()
        {
            assert(NULL!=instance_);
            return *instance_;
        }
        
        
        
    protected:
        inline explicit singleton() throw() {}
        
        inline virtual ~singleton() throw() {}
        
        static inline void suppress_() throw()
        {
            YACK_LOCK(access);
            if(instance_)
            {
                YACK_LOCK(access);
                destroy(NULL);
            }
        }
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(singleton);
        static inline void destroy(void*) throw()
        {
            if(instance_)
            {
                out_of_reach::zset(destructed(instance_),sizeof(CLASS));
                instance_ = NULL;
            }
        }
        
        
        
        
        static CLASS       *instance_;
        
    };
    
    template <class CLASS> CLASS *            singleton<CLASS>::instance_ = NULL;
    template <class CLASS> concurrent::mutex  singleton<CLASS>::access;

}

#endif

