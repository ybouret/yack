
//! \file

#ifndef YACK_AUTO_PTR_INCLUDED
#define YACK_AUTO_PTR_INCLUDED 1

#include "yack/ptr.hpp"

namespace yack
{
    
    template <typename T>
    class auto_ptr : public ptr<T>
    {
    public:
        typedef typename ptr<T>::type         type;
        typedef typename ptr<T>::mutable_type mutable_type;
        using ptr<T>::pointee;
        
        inline type *yield() throw()
        {
            type *old = pointee;
            pointee = 0;
            return old;
        }
        
        inline auto_ptr() throw() : ptr<T>(0) {}
        inline auto_ptr(type *addr) throw() : ptr<T>(addr) {}
        inline auto_ptr(const auto_ptr &_) throw() : ptr<T>( coerce(_).yield() ) {}
        
        inline auto_ptr & operator=(const auto_ptr &other)
        {
            if(this!=&other)
            {
                release();
                pointee = coerce(other).yield();
            }
            return *this;
        }
        
        inline virtual ~auto_ptr() throw() { release(); }
        
        inline auto_ptr & operator=(type *addr)
        {
            if(pointee!=addr)
            {
                release();
                pointee = (mutable_type *)addr;
            }
            return *this;
        }
    private:
        inline void release() throw() {
            if(pointee) { delete pointee; pointee=0; }
        }
        
    };
    
}

#endif
