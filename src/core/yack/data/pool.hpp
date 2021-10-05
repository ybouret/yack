
//! \file

#ifndef YACK_DATA_POOL_INCLUDED
#define YACK_DATA_POOL_INCLUDED 1

#include "yack/data/linked.hpp"

namespace yack
{

    //! check a node for a doubly linked list
#define YACK_POOL_CHECK(NODE) \
assert(NULL!=NODE); assert(NULL==(NODE)->next)

    //__________________________________________________________________________
    //
    //
    //! base class for pool
    //
    //__________________________________________________________________________
    template <typename NODE>
    class pool_of : public interlinked<NODE>
    {
    public:
        //______________________________________________________________________
        //
        // types and definition
        //______________________________________________________________________
        typedef NODE node_type; //!< alias

        //______________________________________________________________________
        //
        // interlinked interface
        //______________________________________________________________________

        bool owns(const NODE *node) const throw()
        {
            assert(node);
            for(const NODE *mine=head;mine;mine=mine->next)
            {
                if(node==mine) return true;
            }
            return false;
        }

        inline virtual void reverse() throw()
        {
            pool_of tmp;
            while(this->size) tmp.store( query() );
            swap_with(tmp);
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        //! store a new node
        NODE *store(NODE *node) throw()
        {
            YACK_POOL_CHECK(node);
            node->next = head;
            head       = node;
            this->increase();
            return node;
        }

        //! query head
        NODE *query() throw()
        {
            assert(0<this->size); assert(NULL!=head);
            NODE *node = head;
            head = head->next;
            node->next = NULL;
            this->decrease();
            return node;
        }

        //! no throw swap
        inline void swap_with(pool_of &other) throw()
        {
            this->xch_size(other);
            cswap(head,other.head);
        }

        //! merge content of other
        inline void merge(pool_of &other) throw()
        {
            while(other.size) store(other.query());
        }

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        NODE *head; //!< the head (top) node

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~pool_of() throw() {} //!< cleanup

    protected:
        explicit pool_of() throw() : interlinked<NODE>(), head(0) {}

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(pool_of);
    };
    
}

#endif
