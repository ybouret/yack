//! \file

#ifndef YACK_DATA_LIST_INCLUDED
#define YACK_DATA_LIST_INCLUDED 1

#include "yack/data/linked.hpp"

namespace yack
{
    
    //! check a node for a doubly linked list
#define YACK_LIST_CHECK(NODE) \
assert(NULL!=NODE); assert(NULL==(NODE)->next); assert(NULL==(NODE)->prev)

    //__________________________________________________________________________
    //
    //
    //! base class for linked list
    //
    //__________________________________________________________________________
    template <typename NODE>
    class list_of : public linked
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~list_of() throw() {}

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        
        //! push_back a new node
        inline NODE *push_back(NODE *node) throw()
        {
            YACK_LIST_CHECK(node);
            if(size<=0)
                return first(node);
            else
            {
                assert(head); assert(tail);
                tail->next = node;
                node->prev = tail;
                tail       = node;
                increase();
                return node;
            }
        }

        //! push_front a new node
        inline NODE *push_front(NODE *node) throw()
        {
            YACK_LIST_CHECK(node);
            if(size<=0)
                return first(node);
            else
            {
                assert(head); assert(tail);
                head->prev = node;
                node->next = head;
                head       = node;
                increase();
                return node;
            }
        }
        
        //! pop  back node
        inline NODE *pop_back() throw()
        {
            switch(size)
            {
                case 1: return last();
                default: break;
            }
            assert(size>1);
            NODE *node = tail;
            tail = tail->prev;
            tail->next = NULL;
            node->prev = NULL;
            decrease();
            YACK_LIST_CHECK(node);
            return node;
        }
        
        //! pop front node
        inline NODE *pop_front() throw()
        {
            switch(size)
            {
                case 1: return last();
                default: break;
            }
            assert(size>1);
            NODE *node = head;
            head = head->next;
            head->prev = NULL;
            node->next = NULL;
            decrease();
            YACK_LIST_CHECK(node);
            return node;
        }
        
        //! check ownership
        inline bool owns(const NODE *node) const throw()
        {
            assert(NULL!=node);
            for(const NODE *mine=head;mine;mine=mine->next)
            {
                if(mine==node) return true;
            }
            return false;
        }
        
        //! access  in [1..size] (slow)
        inline const NODE *get(const size_t indx) const throw() { return get_(indx);          }
        
        //! access in [1..size] (slow)
        inline NODE       *get(const size_t indx)       throw() { return (NODE *) get_(indx); }
        
        //! unlink current node
        inline NODE       *pop(NODE *node) throw()
        {
            assert(owns(node));
            if(head==node)
            {
                return pop_front();
            }
            else
            {
                if(tail==node)
                {
                    return pop_back();
                }
                else
                {
                    assert(size>2);
                    node->next->prev = node->prev;
                    node->prev->next = node->next;
                    node->next = NULL;
                    node->prev = NULL;
                    decrease();
                    return node;
                }
            }
        }
        
        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        
        NODE *head; //!< head NODE
        NODE *tail; //!< tail NODE

    protected:
        //! setup empty
        explicit list_of() throw() : linked(), head(0), tail(0) {}

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(list_of);

        inline NODE *first(NODE *node) throw()
        {
            YACK_LIST_CHECK(node);
            assert(NULL==head); assert(NULL==tail);
            head = tail = node;
            increase();
            return node;
        }
        
        inline NODE *last() throw()
        {
            assert(1==size);
            assert(head!=NULL);
            assert(head==tail);
            NODE *node  = head;
            head = tail = NULL;
            decrease();
            YACK_LIST_CHECK(node);
            return node;
        }
        
        inline const NODE *get_(size_t indx) const throw()
        {
            assert(1<=indx);
            assert(indx<=size);
            const size_t half = size>>1;
            if(indx<=half)
            {
                NODE *node = head;                 assert(node);
                while(--indx>0) { node=node->next; assert(node); }
                return node;
            }
            else
            {
                indx = size-indx;
                NODE *node = tail;                   assert(node);
                while(indx-- > 0) { node=node->prev; assert(node); }
                return node;
            }
        }
        
    };
    

}

#endif

