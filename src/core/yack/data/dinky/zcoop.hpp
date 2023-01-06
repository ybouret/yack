//! \file

#ifndef YACK_DINKY_ZCOOP_INCLUDED
#define YACK_DINKY_ZCOOP_INCLUDED 1

#include "yack/data/dinky/zcache.hpp"
#include "yack/object.hpp"
#include "yack/data/pool.hpp"
#include "yack/lockable.hpp"
#include "yack/counted.hpp"
#include "yack/ptr/arc.hpp"

namespace yack
{
    namespace concurrent
    {
        class mutex;
    }
    
    namespace kernel
    {
        class zcoop : public object, public counted
        {
        public:
            virtual ~zcoop() throw();
            
            lockable & operator*() throw();
            
        protected:
            explicit zcoop();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(zcoop);
            concurrent::mutex *sync;
        };
    }
    
    template <typename NODE>
    class zcoop : public kernel::zcoop, zcache<NODE>
    {
    public:
        typedef arc_ptr< zcoop<NODE> > pointer;
        
        inline explicit zcoop()  : kernel::zcoop(), zcache<NODE>() {}
        inline virtual ~zcoop() throw() { mercy(); }
        
        inline virtual void   release()       throw() { mercy(); }
        inline virtual size_t stowage() const throw() { return pool.size; }
        inline virtual void   reserve(size_t n)
        {
            YACK_LOCK(**this);
            while(n-- > 0) pool.store( object::zacquire<NODE>() );
        }
        
        inline virtual NODE * zquery()
        {
            YACK_LOCK(**this);
            return pool.size ? pool.query() : object::zacquire<NODE>();
        }
        
        inline virtual void   zstore(NODE *node) throw()
        {
            YACK_LOCK(**this);
            pool.store(node);
        }
        
        inline virtual void   devour(list_of<NODE> &live) throw()
        {
            YACK_LOCK(**this);
            while(live.size) zstore( this->turn(live.pop_back()) );
        }
        
        
    private:
        pool_of<NODE> pool;
        inline void   mercy() throw()
        {
            YACK_LOCK(**this);
            while(pool.size) object::zrelease(pool.query());
        }
        YACK_DISABLE_COPY_AND_ASSIGN(zcoop);
    };
}

#endif
