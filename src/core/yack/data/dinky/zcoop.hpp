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
        //! base class for zcoop
        class zcoop : public object, public counted
        {
        public:
            lockable & operator*() noexcept; //!< get mutex
            virtual   ~zcoop() noexcept;     //!< cleanup
            explicit   zcoop();             //!< setup with internal mutex
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(zcoop);
            concurrent::mutex *sync;
        };
    }
    
    //__________________________________________________________________________
    //
    //
    //! coop-cache: cache for cooperative lists
    //
    //__________________________________________________________________________
    template <typename NODE>
    class zcoop : public kernel::zcoop, public zcache<NODE>
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef arc_ptr< zcoop<NODE> > proxy; //!< alias
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit zcoop()  : kernel::zcoop(), zcache<NODE>(), pool() {} //!< setup
        inline virtual ~zcoop() noexcept { mercy(); } //!< cleanup
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        inline virtual void   release()       noexcept { mercy(); }          //!< locked release
        inline virtual size_t stowage() const noexcept { return pool.size; } //!< current reseve
                                                                            
        //! lock and reserve zombies
        inline virtual void   reserve(size_t n)
        {
            YACK_LOCK(**this);
            while(n-- > 0) pool.store( object::zacquire<NODE>() );
        }
        
        //! locked query existing of new zombie NODE
        inline virtual NODE * zquery()
        {
            YACK_LOCK(**this);
            return pool.size ? pool.query() : object::zacquire<NODE>();
        }
        
        //! locked storage of existing zombie NODE
        inline virtual void   zstore(NODE *node) noexcept
        {
            YACK_LOCK(**this);
            pool.store(node);
        }
        
        //! locked devouring or live list
        inline virtual void   devour(list_of<NODE> &live) noexcept
        {
            YACK_LOCK(**this);
            while(live.size) zstore( this->turn(live.pop_back()) );
        }
        
        
    private:
        pool_of<NODE> pool;
        inline void   mercy() noexcept
        {
            YACK_LOCK(**this);
            while(pool.size) object::zrelease(pool.query());
        }
        YACK_DISABLE_COPY_AND_ASSIGN(zcoop);
    };
}

#endif
