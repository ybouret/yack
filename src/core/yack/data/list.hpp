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
    class list_of : public interlinked<NODE>
    {
    public:
        //______________________________________________________________________
        //
        // types and definition
        //______________________________________________________________________
        typedef NODE node_type; //!< alias
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup empty
        explicit list_of() throw() : interlinked<NODE>(), head(0), tail(0) {}

        //! need cleanup before!
        virtual ~list_of() throw() {}
        
        //______________________________________________________________________
        //
        // interlinked interface
        //______________________________________________________________________
        inline virtual bool owns(const NODE *node) const throw()
        {
            assert(NULL!=node);
            for(const NODE *mine=head;mine;mine=mine->next)
            {
                if(mine==node) return true;
            }
            return false;
        }

        inline virtual void reverse() throw()
        {
            list_of tmp;
            while(this->size) tmp.push_front( pop_front() );
            swap_with(tmp);
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        
        //! no-throw exchange with another list
        inline void swap_with(list_of &other) throw()
        {
            this->xch_size(other);
            cswap(head,other.head);
            cswap(tail,other.tail);
        }
        
        //! push_back a new node
        inline NODE *push_back(NODE *node) throw()
        {
            YACK_LIST_CHECK(node);
            if(this->size<=0)
                return first(node);
            else
            {
                assert(head); assert(tail);
                tail->next = node;
                node->prev = tail;
                tail       = node;
                this->increase();
                return node;
            }
        }

        //! push_front a new node
        inline NODE *push_front(NODE *node) throw()
        {
            YACK_LIST_CHECK(node);
            if(this->size<=0)
                return first(node);
            else
            {
                assert(head); assert(tail);
                head->prev = node;
                node->next = head;
                head       = node;
                this->increase();
                return node;
            }
        }
        
        //! pop  back node
        inline NODE *pop_back() throw()
        {
            switch(this->size)
            {
                case 1: return last();
                default: break;
            }
            assert(this->size>1);
            NODE *node = tail;
            tail = tail->prev;
            tail->next = NULL;
            node->prev = NULL;
            this->decrease();
            YACK_LIST_CHECK(node);
            return node;
        }
        
        //! pop front node
        inline NODE *pop_front() throw()
        {
            switch(this->size)
            {
                case 1: return last();
                default: break;
            }
            assert(this->size>1);
            NODE *node = head;
            head = head->next;
            head->prev = NULL;
            node->next = NULL;
            this->decrease();
            YACK_LIST_CHECK(node);
            return node;
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
                    assert(this->size>2);
                    return pull(node);
                }
            }
        }
        
        //! move owned node to head node
        inline NODE *move_to_front(NODE *node) throw()
        {
            assert(owns(node));
            if(head==node)
            {
                return node;
            }
            else
            {
                if(tail==node) return push_front( pop_back() );
                else           return push_front( pull(node) );
            }
        }
        
        //! insert a new node after an owned node
        inline NODE *insert_after(NODE *mine,
                                  NODE *node) throw()
        {
            assert(owns(mine));
            YACK_LIST_CHECK(node);
            if(mine==tail)
            {
                return push_back(node);
            }
            else
            {
                NODE *next = mine->next;
                mine->next = node; node->next = next;
                next->prev = node; node->prev = mine;
                this->increase();
                return node;
            }
        }
        
        //! insert a new node before an owned node
        inline NODE *insert_before(NODE *mine,
                                   NODE *node) throw()
        {
            assert(owns(mine));
            YACK_LIST_CHECK(node);
            if(mine==head)
            {
                return push_front(node);
            }
            else
            {
                NODE *prev = mine->prev;
                prev->next = node; node->next = mine;
                mine->prev = node; node->prev = prev;
                this->increase();
                return node;
            }
        }
        
        //! move current node!=head towards head
        inline NODE *towards_front(NODE *node) throw()
        {
            assert(owns(node));
            assert(NULL!=node->prev);
            assert(head!=node);
            NODE *mine = node->prev;
            return insert_before(mine,pop(node));
        }

        //! merge back
        void merge_back(list_of &other) throw()
        {
            if(this->size<=0)
            {
                swap_with(other);
            }
            else
            {
                if(other.size>0)
                {
                    tail->next = other.head;
                    other.head->prev = tail;
                    tail = other->tail;
                    this->size += other.size;
                    other.hard_reset_();
                }
            }
        }
        
        
        
        
        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        
        NODE *head; //!< head NODE
        NODE *tail; //!< tail NODE



    private:
        YACK_DISABLE_COPY_AND_ASSIGN(list_of);

        inline NODE *first(NODE *node) throw()
        {
            YACK_LIST_CHECK(node);
            assert(NULL==head); assert(NULL==tail);
            head = tail = node;
            this->increase();
            return node;
        }
        
        inline NODE *last() throw()
        {
            assert(1==this->size);
            assert(head!=NULL);
            assert(head==tail);
            NODE *node  = head;
            head = tail = NULL;
            this->decrease();
            YACK_LIST_CHECK(node);
            return node;
        }
        
        inline NODE *pull(NODE *node) throw()
        {
            node->next->prev = node->prev;
            node->prev->next = node->next;
            node->next = NULL;
            node->prev = NULL;
            this->decrease();
            return node;
        }
        
        
        inline const NODE *get_(size_t indx) const throw()
        {
            assert(1<=indx);
            assert(indx<=this->size);
            const size_t half = this->size>>1;
            if(indx<=half)
            {
                NODE *node = head;                 assert(node);
                while(--indx>0) { node=node->next; assert(node); }
                return node;
            }
            else
            {
                indx       = this->size-indx;
                NODE *node = tail;                   assert(node);
                while(indx-- > 0) { node=node->prev; assert(node); }
                return node;
            }
        }

    protected:
        //! hard reset for internal operations
        inline void hard_reset_() throw()
        {
            coerce(this->size) = 0;
            head = 0;
            tail = 0;
        }

    };
    

}

#endif

