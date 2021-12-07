//! \file

#ifndef YACK_ARC_PTR_INCLUDED
#define YACK_ARC_PTR_INCLUDED 1

#include "yack/ptr/counted.hpp"

namespace yack
{


    //__________________________________________________________________________
    //
    //! counted pointer of counted object
    /**
     * pointee must have void withhold(), bool liberate() and size_t quantity()
     */
    //__________________________________________________________________________
    template <typename T>
    class arc_ptr : public counted_ptr<T>
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef typename ptr<T>::type         type;         //!< alias
        typedef typename ptr<T>::mutable_type mutable_type; //!< alias
        using ptr<T>::pointee;
        
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup with valid address
        inline arc_ptr(type *addr) throw() : counted_ptr<T>(addr)
        {
            assert(pointee);
            pointee->withhold();
        }


        //! no-throw copy
        inline arc_ptr(const arc_ptr &_) throw() : counted_ptr<T>( coerce(_).pointee )
        {
            pointee->withhold();
        }

        //! no-throw assign by copy/swap
        inline arc_ptr & operator=(const arc_ptr &other)
        {
            arc_ptr temp(other);
            cswap(pointee,temp.pointee);
            return *this;
        }
        
        //! cleanup
        inline virtual ~arc_ptr() throw()
        {
            assert(pointee);
            if(pointee->liberate())
            {
                delete pointee;
            }
            pointee = NULL;
        }

        //! no-throw inline exchange
        inline void commute(arc_ptr &other) throw()
        {
            cswap(pointee,other.pointee);
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! counted_ptr interface
        virtual size_t references() const throw()
        {
            assert(pointee);
            return pointee->quantity();
        }

    };

    
}

#endif
