//! \file

#ifndef YACK_DATA_ZPOOL_INCLUDED
#define YACK_DATA_ZPOOL_INCLUDED 1

#include "yack/data/pool.hpp"
#include "yack/object.hpp"
#include "yack/container/releasable.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! pool with zombie NODEs
    //
    //__________________________________________________________________________
    template <typename NODE>
    class zpool_of : public pool_of<NODE>, public releasable
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit zpool_of() throw() : list_of<NODE>()  {}
        inline virtual ~zpool_of() throw() { clear(); }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        virtual void release() throw() { clear(); }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(zpool_of);
        inline void clear() throw()
        {
            while(this->size) object::zrelease(this->query());
        }
    };

}

#endif
