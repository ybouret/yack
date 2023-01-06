
//! \file

#ifndef YACK_DATA_SLIM_ROOT_INCLUDED
#define YACK_DATA_SLIM_ROOT_INCLUDED 1

#include "yack/data/list.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! root for list of anonymous nodes
    //
    //__________________________________________________________________________
    template <typename NODE>
    class slim_root : public list_of<NODE>
    {
    public:
        using list_of<NODE>::pop;
        using list_of<NODE>::pop_back;
        using list_of<NODE>::pop_front;

        //! clear content
        inline void clear() throw() { kill_all(); }

        //! remove a node
        inline void cut_node(NODE *node) throw() { kill_one(  pop(node) );  }

        //! cut tail node
        inline void cut_tail() throw()           { kill_one(  pop_back() ); }

        //! cut head node
        inline void cut_head() throw()           { kill_one( pop_front() ); }

        virtual ~slim_root() throw() {}

    protected:
        explicit slim_root() throw() : list_of<NODE>() {}

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(slim_root);
        virtual void kill_one(NODE *) throw() = 0;
        virtual void kill_all()       throw() = 0;
    };
}

#endif
