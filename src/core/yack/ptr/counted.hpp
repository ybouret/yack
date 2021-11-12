
//! \file

#ifndef YACK_COUNTED_PTR_INCLUDED
#define YACK_COUNTED_PTR_INCLUDED 1

#include "yack/ptr.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! base class for counted pointed (arc,shared...)
    //
    //__________________________________________________________________________
    template <typename T>
    class counted_ptr : public ptr<T>
    {
    public:
        typedef typename ptr<T>::type type;             //!< alias
        virtual size_t  references() const throw() = 0; //!< interface
        inline virtual ~counted_ptr() throw() {}        //!< cleanup

    protected:
        //! setup by forwarding
        inline explicit counted_ptr(type *addr) throw() : ptr<T>(addr) {}

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(counted_ptr);
    };
}
#endif
