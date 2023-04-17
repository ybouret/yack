//! \file

#ifndef YACK_DATA_LIST_INCLUDED
#define YACK_DATA_LIST_INCLUDED 1

#include "yack/data/linked.hpp"
#include "yack/data/list/core.hpp"

namespace yack
{


    //__________________________________________________________________________
    //
    //! check a node for a doubly linked list
    //__________________________________________________________________________
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
        typedef NODE node_type;         //!< alias
        using interlinked<NODE>::head;
        using linked::size;
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup empty
        inline explicit list_of() noexcept : interlinked<NODE>(), tail(0) {}

        //! setup from compact state
        inline explicit list_of(core_list_of<NODE> &io) noexcept :
        interlinked<NODE>(), tail(io.tail)
        {
            /** update */           io.tail = 0;
            head         = io.head; io.head = 0;
            coerce(size) = io.size; io.size = 0;
        }

        //! need cleanup before!
        inline virtual ~list_of() noexcept {}

        //______________________________________________________________________
        //
        //! saving compact state
        //______________________________________________________________________
        inline void save(core_list_of<NODE> &io) noexcept
        {
            io.head=head; head         = NULL;
            io.tail=tail; tail         = NULL;
            io.size=size; coerce(size) = 0; }

        //______________________________________________________________________
        //
        // interlinked interface
        //______________________________________________________________________

        //! sequential ownership test
        inline virtual bool owns(const NODE *node) const noexcept
        {
            for(const NODE *mine=head;mine;mine=mine->next)
            {
                if(mine==node) return true;
            }
            return false;
        }

        //! reverse by auxiliary list
        inline virtual void reverse() noexcept
        {
            list_of tmp;
            while(size) tmp.push_front( pop_front() );
            swap_with(tmp);
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        
        //! no-throw exchange with another list
        inline void swap_with(list_of &other) noexcept
        {
            this->xch_size(other);
            cswap(head,other.head);
            cswap(tail,other.tail);
        }
        
        //! push_back a new node
        inline NODE *push_back(NODE *node) noexcept
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
                this->increase();
                return node;
            }
        }

        
        //! push_front a new node
        inline NODE *push_front(NODE *node) noexcept
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
                this->increase();
                return node;
            }
        }
        
        //! pop  back node
        inline NODE *pop_back() noexcept
        {
            switch(size)
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
        inline NODE *pop_front() noexcept
        {
            switch(size)
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
        inline const NODE *get(const size_t indx) const noexcept { return get_(indx);          }
        
        //! access in [1..size] (slow)
        inline NODE       *get(const size_t indx)       noexcept { return (NODE *) get_(indx); }
        
        //! unlink current node
        inline NODE       *pop(NODE *node) noexcept
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
                    return pull(node);
                }
            }
        }

        //! replace mine with another node, return mine for further processing
        inline NODE *replace(NODE *mine, NODE *node) noexcept
        {
            assert(NULL!=mine); assert(NULL!=node); assert(NULL==node->prev); assert(NULL==node->next);
            assert( owns(mine) );

            if(mine==head)
            {
                (void) pop_front();
                (void) push_front(node);
            }
            else
            {
                if(mine==tail)
                {
                    (void) pop_back();
                    (void) push_back(node);
                }
                else
                {
                    assert(size>2);
                    mine->next->prev = node; node->next = mine->next;
                    mine->prev->next = node; node->prev = mine->prev;
                    mine->next = NULL;
                    mine->prev = NULL;
                }
            }


            return mine;
        }

        //! move owned node to head node
        inline NODE *move_to_front(NODE *node) noexcept
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
                                  NODE *node) noexcept
        {
            assert(owns(mine));
            YACK_LIST_CHECK(node);
            if(mine==tail)
            {
                return push_back(node);
            }
            else
            {
                NODE *next = mine->next; assert(NULL!=next);
                mine->next = node; node->next = next;
                next->prev = node; node->prev = mine;
                this->increase();
                return node;
            }
        }

        //! insert a sub list after an owned node
        inline void insert_after(NODE          *mine,
                                 list_of<NODE> &sub) noexcept
        {
            assert(owns(mine));
            if(mine==tail)
            {
                merge_back(sub);
            }
            else
            {
                if(sub.size)
                {
                    // link
                    NODE *next = mine->next; assert(NULL!=next);
                    mine->next = sub.head; sub.head->prev = mine;
                    next->prev = sub.tail; sub.tail->next = next;

                    // update
                    coerce(size) += sub.size;
                    sub.hard_reset_();
                }
            }
        }

        
        //! insert a new node before an owned node
        inline NODE *insert_before(NODE *mine,
                                   NODE *node) noexcept
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
        inline NODE *towards_front(NODE *node) noexcept
        {
            assert(owns(node));
            assert(NULL!=node->prev);
            assert(head!=node);
            NODE *mine = node->prev;
            return insert_before(mine,pop(node));
        }

        //! merge back
        void merge_back(list_of &other) noexcept
        {
            if(size<=0)
            {
                swap_with(other);
            }
            else
            {
                if(other.size>0)
                {
                    tail->next       = other.head;
                    other.head->prev = tail;
                    tail             = other.tail;
                    coerce(size)    += other.size;
                    other.hard_reset_();
                }
            }
        }

        //! merge front
        void merge_front(list_of &other) noexcept
        {
            other.merge_back(*this);
            swap_with(other);
        }


        //! rolling down content
        inline void roll_down() noexcept
        {
            switch(size)
            {
                case 0:
                case 1:
                    return;

                default:
                    push_back( pop_front() );
            }
        }

        //! rolling up content
        inline void roll_up() noexcept
        {
            switch(size)
            {
                case 0:
                case 1:
                    return;

                default:
                    push_front( pop_back() );
            }
        }

        //! store by increasing memory
        NODE *store_increasing_memory(NODE *node) noexcept
        {
            YACK_LIST_CHECK(node);
            assert(false==owns(node));
            assert(this->memory_is_increasing());
            if(size<=0)
            {
                return first(node);
            }
            else
            {
                if(node<head)
                {
                    return push_front(node);
                }
                else
                {
                    if(tail<node)
                    {
                        return push_back(node);
                    }
                    else
                    {
                        assert(size>=2);
                        NODE *gt = tail;
                        NODE *lt = gt->prev; assert(lt<gt);
                        while(true)
                        {
                            assert(lt);
                            assert(gt);
                            assert(lt<gt);
                            assert(node<gt);
                            
                            if(node>lt)
                            {
                                lt->next = node; node->prev = lt;
                                gt->prev = node; node->next = gt;
                                this->increase();
                                assert(this->memory_is_increasing());
                                return node;
                            }
                            gt=lt;
                            lt=lt->prev;
                        }
                    }
                }
            }
        }



        
        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        NODE *tail; //!< tail NODE



    private:
        YACK_DISABLE_COPY_AND_ASSIGN(list_of);

        inline NODE *first(NODE *node) noexcept
        {
            YACK_LIST_CHECK(node);
            assert(NULL==head); assert(NULL==tail);
            head = tail = node;
            this->increase();
            return node;
        }
        
        inline NODE *last() noexcept
        {
            assert(1==size);
            assert(head!=NULL);
            assert(head==tail);
            NODE *node  = head;
            head = tail = NULL;
            this->decrease();
            YACK_LIST_CHECK(node);
            return node;
        }
        
        inline NODE *pull(NODE *node) noexcept
        {
            node->next->prev = node->prev;
            node->prev->next = node->next;
            node->next = NULL;
            node->prev = NULL;
            this->decrease();
            return node;
        }
        
        
        inline const NODE *get_(size_t indx) const noexcept
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
                indx       = size-indx;
                NODE *node = tail;                   assert(node);
                while(indx-- > 0) { node=node->prev; assert(node); }
                return node;
            }
        }
        
      

    protected:
        //! hard reset for internal operations
        inline void hard_reset_() noexcept
        {
            coerce(size) = 0;
            head = 0;
            tail = 0;
        }

    };
    

}

#endif

