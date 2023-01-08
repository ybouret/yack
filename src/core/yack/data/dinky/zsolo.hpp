

//! \file

#ifndef YACK_DINKY_ZSOLO_INCLUDED
#define YACK_DINKY_ZSOLO_INCLUDED 1

#include "yack/data/dinky/zcache.hpp"
#include "yack/data/dinky/zproxy.hpp"
#include "yack/object.hpp"
#include "yack/data/pool.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! solo-cache: use internal cache for self-contained classes
    //
    //__________________________________________________________________________
    template <typename NODE>
    class zsolo : public zcache<NODE>
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef zproxy< zsolo<NODE> > proxy; //!< alias
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit zsolo() throw() : zcache<NODE>() {} //!< setup
        inline virtual ~zsolo() throw() { mercy(); }        //!< cleanup
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        inline virtual void   release()       throw() { mercy(); }          //!< release pool
        inline virtual size_t stowage() const throw() { return pool.size; } //!< pool size
                                                                            
        //! store zombies into pool
        inline virtual void   reserve(size_t n)
        {
            while(n-- > 0) zstore( object::zacquire<NODE>() );
        }
        
        //! get existing or new zombie
        inline virtual NODE * zquery() { return pool.size? pool.query() : object::zacquire<NODE>(); }
        
        //! store existing zombie NODE
        inline virtual void   zstore(NODE *node) throw() { pool.store(node); }
        
        //! turn and store all alive
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
