//! \file

#ifndef YACK_ARC_PTR_INCLUDED
#define YACK_ARC_PTR_INCLUDED 1

#include "yack/ptr.hpp"

namespace yack
{
    
    template <typename T>
    class arc_ptr : public ptr<T>
    {
    public:
        typedef typename ptr<T>::type         type;
        typedef typename ptr<T>::mutable_type mutable_type;
        using ptr<T>::pointee;
        
        
        
        inline arc_ptr(type *addr) throw() : ptr<T>(addr)
        {
            assert(pointee);
            pointee->withhold();
        }
        
        inline arc_ptr(const arc_ptr &_) throw() : ptr<T>( coerce(_).pointee )
        {
            pointee->withhold();
        }
        
        inline arc_ptr & operator=(const arc_ptr &other)
        {
            arc_ptr temp(other);
            cswap(pointee,temp.pointee);
            return *this;
        }
        
        
        inline virtual ~arc_ptr() throw()
        {
            assert(pointee);
            if(pointee->liberate())
            {
                delete pointee;
            }
            pointee = NULL;
        }
        
    };
    
}

#endif
