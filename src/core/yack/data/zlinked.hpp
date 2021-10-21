
//! \file

#ifndef YACK_DATA_ZLINKED_INCLUDED
#define YACK_DATA_ZLINKED_INCLUDED 1


#include "yack/object.hpp"
#include "yack/container/releasable.hpp"

namespace yack
{
    template <typename NODE>
    class zlinked : public releasable
    {
    public:
        inline virtual ~zlinked() throw() {}

        virtual NODE  *zquery() = 0;
        virtual void   zstore(NODE *) throw() = 0;


    protected:
        inline explicit     zlinked() throw() : releasable() {}
        inline static NODE *zcreate()                   { return object::zacquire<NODE>(); }
        inline static void  zdelete(NODE *node) throw() { object::zrelease(node);          }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(zlinked);
    };
}

#endif
