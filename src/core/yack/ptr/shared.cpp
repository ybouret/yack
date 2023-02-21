
#include "yack/ptr/shared.hpp"
#include "yack/object.hpp"

namespace yack
{

    static const size_t block_size = sizeof(counted_ptr<int>);

    size_t * shared_ptr_ref:: acquire()
    {
        size_t *ref = static_cast<size_t*>( object::operator new(block_size) );
        assert(ref);
        assert(0==*ref);
        *ref = 1;
        return ref;
    }

    void shared_ptr_ref:: release(size_t *&ref) noexcept
    {
        assert(ref);
        assert(0==*ref);
        object:: operator delete(ref,block_size);
        ref=NULL;
    }

}


