//! \file

#ifndef YACK_DATA_ZLIST_INCLUDED
#define YACK_DATA_ZLIST_INCLUDED 1

#include "yack/data/list.hpp"
#include "yack/data/zlinked.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! list with zombie NODEs
    //
    //__________________________________________________________________________
    template <typename NODE>
    class zlist_of :  public zlinked<NODE>
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit zlist_of() throw() : zlinked<NODE>(), impl() {}
        inline virtual ~zlist_of() throw() { clear(); }

        //______________________________________________________________________
        //
        // zlinked interface
        //______________________________________________________________________
        inline virtual NODE * zquery() { return impl.size ? impl.pop_back() : this->zcreate(); }
        inline virtual size_t size() const throw() { return impl.size; }

        //______________________________________________________________________
        //
        // releasable interface
        //______________________________________________________________________
        virtual void release() throw() { clear(); }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(zlist_of);
        list_of<NODE> impl;
        inline virtual void  _zstore(NODE *node) throw() { assert(node); impl.push_back(node); }

        inline void clear() throw()
        {
            while(impl.size) this->zdelete(impl.pop_back());
        }
    };

}

#endif
