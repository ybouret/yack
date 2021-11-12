
//! \file

#ifndef YACK_SHARED_PTR_INCLUDED
#define YACK_SHARED_PTR_INCLUDED 1

#include "yack/ptr/counted.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! memory for reference
    //
    //__________________________________________________________________________
    struct shared_ptr_ref
    {
        static size_t * acquire();                   //!< acquire, set  to 1
        static void     release(size_t * &) throw(); //!< release, must be 0
    };

    //__________________________________________________________________________
    //
    //
    //! counted pointer of any object, create internal reference
    //
    //__________________________________________________________________________
    template <typename T>
    class shared_ptr : public counted_ptr<T>
    {
    public:
        //______________________________________________________________________
        //
        // types and definition
        //______________________________________________________________________
        typedef typename ptr<T>::type         type;          //!< alias
        typedef typename ptr<T>::mutable_type mutable_type;  //!< alias
        using ptr<T>::pointee;


        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup with valid address
        inline shared_ptr(type *addr) : counted_ptr<T>(addr), ref_ptr(0)
        {
            assert(pointee);
            try { ref_ptr = shared_ptr_ref::acquire();        }
            catch(...) { delete pointee; pointee=NULL; throw; }
        }

        //! no-throw copy
        inline shared_ptr(const shared_ptr &other) throw() :
        counted_ptr<T>(other.pointee),
        ref_ptr(other.ref_ptr)
        {
            assert(ref_ptr);
            ++(*ref_ptr);
        }

        //! no-throw assign by copy/swap
        inline shared_ptr & operator=(const shared_ptr &other) throw()
        {
            shared_ptr temp(other);
            cswap(temp.pointee,pointee);
            cswap(temp.ref_ptr,ref_ptr);
            return *this;
        }

        //! cleanup
        inline virtual ~shared_ptr() throw()
        {
            assert(ref_ptr);
            assert(*ref_ptr>0);
            if( --(*ref_ptr) <= 0)
            {
                shared_ptr_ref::release(ref_ptr);
                delete pointee;
                pointee = NULL;
            }
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! counted_ptr interface
        inline virtual size_t references() const throw() { assert(ref_ptr); return *ref_ptr; }


    private:
        size_t *ref_ptr;
    };


}

#endif
