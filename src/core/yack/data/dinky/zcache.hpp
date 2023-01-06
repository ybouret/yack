//! \file

#ifndef YACK_DINKY_ZCACHE_INCLUDED
#define YACK_DINKY_ZCACHE_INCLUDED 1

#include "yack/data/list.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/container/releasable.hpp"
#include "yack/type/destruct.hpp"

namespace yack
{
    
    
    template <typename NODE>
    class zcache : public releasable
    {
    public:
        
        // NODE interface
        virtual void   zstore(NODE *)  throw()         = 0; //!< store a zombie node
        virtual NODE  *zquery()                        = 0; //!< query a new zombie node
        virtual void   devour(list_of<NODE> &) throw() = 0;
       
        // cache management
        virtual size_t stowage() const throw()         = 0; //!< available
        virtual void   reserve(size_t)                 = 0;
        
        //C++
        inline virtual ~zcache() throw() {}
        
    protected:
        inline explicit zcache() throw() : releasable() {}
        
        inline static NODE *turn(NODE *live) throw()
        {
            assert(live);
            return static_cast<NODE *>(out_of_reach:: naught( destructed(live) ));
        }
        
        
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(zcache);
    };
}

#endif

