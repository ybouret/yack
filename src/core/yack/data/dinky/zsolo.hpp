

//! \file

#ifndef YACK_DINKY_ZSOLO_INCLUDED
#define YACK_DINKY_ZSOLO_INCLUDED 1

#include "yack/data/dinky/zcache.hpp"
#include "yack/object.hpp"
#include "yack/data/pool.hpp"

namespace yack
{
    
    template <typename NODE>
    class zsolo : public zcache<NODE>
    {
    public:
        inline explicit zsolo() throw() : zcache<NODE>() {}
        inline virtual ~zsolo() throw() { release(); }
        
        inline virtual void   release()       throw() { mercy(); }
        inline virtual size_t stowage() const throw() { return pool.size; }
        inline virtual void   reserve(size_t n)
        {
            while(n-- > 0) zstore( object::zacquire<NODE>() );
        }
        
        inline virtual NODE * zquery() { return pool.size? pool.query() : object::zacquire<NODE>(); }
        inline virtual void   zstore(NODE *node) throw() { pool.store(node); }
        inline virtual void   devour(list_of<NODE> &live) throw()
        {
            while(live.size) zstore( this->turn(live.pop_back()) );
        }
        
        
    private:
        pool_of<NODE> pool;
        inline void   mercy() throw() { while(pool.size) object::zrelease(pool.query()); }
        YACK_DISABLE_COPY_AND_ASSIGN(zsolo);
    };
}

#endif
