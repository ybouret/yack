
//! \file

#ifndef YACK_DINKY_ZNONE_INCLUDED
#define YACK_DINKY_ZNONE_INCLUDED 1

#include "yack/data/dinky/zcache.hpp"
#include "yack/object.hpp"

namespace yack
{
    
    template <typename NODE>
    class znone : public zcache<NODE>
    {
    public:
        inline explicit znone() throw() : zcache<NODE>() {}
        inline virtual ~znone() throw() {}
        
        inline virtual void   release() throw() {}
        inline virtual size_t stowage() const throw() { return 0; }
        inline virtual void   reserve(size_t) {}
        inline virtual NODE * zquery() { return object::zacquire<NODE>(); }
        inline virtual void   zstore(NODE *node) throw() { object::zrelease(node); }
        inline virtual void   devour(list_of<NODE> &live) throw()
        {
            while(live.size) zstore( this->turn(live.pop_back()) );
        }
        
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(znone);
    };
}

#endif
