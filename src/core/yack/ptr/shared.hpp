
//! \file

#ifndef YACK_SHARED_PTR_INCLUDED
#define YACK_SHARED_PTR_INCLUDED 1

#include "yack/ptr/counted.hpp"

namespace yack
{

    struct shared_ptr_ref
    {
        size_t * acquire();
        void     release() throw();
    };

    //! counted pointer of object with internal reference
    template <typename T>
    class shared_ptr : public counted_ptr<T>
    {
    public:
        typedef typename ptr<T>::type         type;
        typedef typename ptr<T>::mutable_type mutable_type;
        using ptr<T>::pointee;


    private:
        size_t *ref_;

    };


}

#endif
