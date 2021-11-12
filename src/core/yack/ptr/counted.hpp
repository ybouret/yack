
//! \file

#ifndef YACK_COUNTED_PTR_INCLUDED
#define YACK_COUNTED_PTR_INCLUDED 1

#include "yack/ptr.hpp"

namespace yack
{

    template <typename T>
    class counted_ptr : public ptr<T>
    {
    public:
        typedef typename ptr<T>::type type;

        inline virtual ~counted_ptr() throw() {}

        virtual size_t references() const throw() = 0;

    protected:
        inline explicit counted_ptr(type *addr) throw() : ptr<T>(addr) {}

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(counted_ptr);
    };
}
#endif
