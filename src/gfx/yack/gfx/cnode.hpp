
//! \file

#ifndef YACK_GFX_CNODE_INCLUDED
#define YACK_GFX_CNODE_INCLUDED 1

#include "yack/gfx/coord.hpp"
#include "yack/memory/small/exclusive.hpp"
#include "yack/data/list/concrete.hpp"

namespace yack
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        //! coordinate node
        //
        //______________________________________________________________________
        class cnode
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            YACK_EXCLUSIVE_DECL(cnode,999); //!< for memory

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            cnode(const coord &) noexcept; //!< setup
            ~cnode()             noexcept; //!< cleanup
            cnode(const cnode &) noexcept; //!< copy
            const coord & operator*() const noexcept; //!< access

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            cnode * next; //!< for list
            cnode * prev; //!< for list

        private:
            YACK_DISABLE_ASSIGN(cnode);
            const  coord  pos;
        };

        //! list of constant coord
        typedef klist<const coord,cnode> cnodes;

    }
}

#endif

