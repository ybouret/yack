
//! \file

#ifndef YACK_GFX_CNODE_INCLUDED
#define YACK_GFX_CNODE_INCLUDED 1

#include "yack/gfx/coord.hpp"
#include "yack/memory/small/exclusive.hpp"
#include "yack/data/list/concrete.hpp"
#include "yack/data/pool/concrete.hpp"

namespace yack
{
    namespace graphic
    {
        //! coordinate node
        class cnode
        {
        public:
            YACK_EXCLUSIVE_DECL(cnode,999);
            typedef klist<const coord,cnode> list;
            typedef kpool<const coord,cnode> pool;

            cnode       * next;
            cnode       * prev;

            const coord & operator*() const throw();

            cnode(const coord &) throw();
            ~cnode() throw();
            cnode(const cnode &) throw();

        private:
            YACK_DISABLE_ASSIGN(cnode);
            const  coord  pos;
        };

    }
}

#endif

