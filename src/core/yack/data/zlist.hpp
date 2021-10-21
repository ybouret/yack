
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
    class zlist_of : public list_of<NODE>, public zlinked<NODE>
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit zlist_of() throw() : list_of<NODE>()  {}
        inline virtual ~zlist_of() throw() { clear(); }

        inline virtual NODE *zquery()                   { return this->size ? this->pop_back() : this->zcreate(); }
        inline virtual void  zstore(NODE *node) throw() { assert(node); this->push_back(node); }

        //______________________________________________________________________
        //
        // releasable interface
        //______________________________________________________________________
        virtual void release() throw() { clear(); }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(zlist_of);
        inline void clear() throw()
        {
            while(this->size) this->zdelete(this->pop_back());
        }
    };

}

#endif
