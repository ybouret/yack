//! \file

#ifndef YACK_ARC_PTR_INCLUDED
#define YACK_ARC_PTR_INCLUDED 1

#include "yack/ptr/counted.hpp"

namespace yack
{


    //! counted pointer of counted object
    /**
     * pointee must have void withhold(), bool liberate() and size_t quantity()
     */
    template <typename T>
    class arc_ptr : public counted_ptr<T>
    {
    public:
        typedef typename ptr<T>::type         type;
        typedef typename ptr<T>::mutable_type mutable_type;
        using ptr<T>::pointee;
        
        
        
        inline arc_ptr(type *addr) throw() : counted_ptr<T>(addr)
        {
            assert(pointee);
            pointee->withhold();
        }
        
        inline arc_ptr(const arc_ptr &_) throw() : counted_ptr<T>( coerce(_).pointee )
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

        virtual size_t references() const throw()
        {
            assert(pointee);
            return pointee->quantity();
        }

    };

    
}

#endif
