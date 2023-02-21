
//! \file

#ifndef YACK_DATA_POOL_INCLUDED
#define YACK_DATA_POOL_INCLUDED 1

#include "yack/data/linked.hpp"
#include "yack/data/pool/core.hpp"

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
        using interlinked<NODE>::head;
        using linked::size;
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~pool_of() noexcept {} //!< need cleanup before this!
        
        //! setup empty
        inline explicit pool_of() noexcept : interlinked<NODE>() {}
        
        //! setup from state
        inline explicit pool_of(core_pool_of<NODE> &io) noexcept :
        interlinked<NODE>()
        {
            head         = io.head; io.head = 0;
            coerce(size) = io.size; io.size = 0;
        }
        
        
        //______________________________________________________________________
        //
        // interlinked interface
        //______________________________________________________________________
        
        //! test ownership
        inline virtual bool owns(const NODE *node) const noexcept
        {
            assert(node);
            for(const NODE *mine=head;mine;mine=mine->next)
            {
                if(node==mine) return true;
            }
            return false;
        }
        
        //! reversing
        inline virtual void reverse() noexcept
        {
            pool_of tmp;
            while(size) tmp.store( query() );
            swap_with(tmp);
        }
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        //! store a new node
        NODE *store(NODE *node) noexcept
        {
            YACK_POOL_CHECK(node);
            node->next = head;
            head       = node;
            this->increase();
            return node;
        }

        //! last node for size>0
        NODE *tail() noexcept
        {
            assert(size>0);
            NODE *curr = head;
            while(curr->next)
                curr=curr->next;
            return curr;
        }

        //! const last node for size>0
        const NODE *tail() const noexcept
        {
            assert(size>0);
            const NODE *curr = head;
            while(curr->next)
                curr=curr->next;
            return curr;
        }

        //! stash a new node
        NODE *stash(NODE *node) noexcept
        {
            YACK_POOL_CHECK(node);
            if(size>0)
            {
                tail()->next = node;
                this->increase();
                return node;
            }
            else
            {
                return store(node);
            }
        }
        
        //! query head
        NODE *query() noexcept
        {
            assert(0<this->size); assert(NULL!=head);
            NODE *node =  head;
            head       =  head->next;
            node->next = NULL;
            this->decrease();
            return node;
        }
        
        //! no throw swap
        inline void swap_with(pool_of &other) noexcept
        {
            this->xch_size(other);
            cswap(head,other.head);
        }
        
        //! merge content of other
        inline void merge(pool_of &other) noexcept
        {
            while(other.size) store(other.query());
        }
        
        //______________________________________________________________________
        //
        //! save state
        //______________________________________________________________________
        inline void save(core_pool_of<NODE> &io) noexcept
        {
            io.head = head; head=NULL;
            io.size = size; coerce(size)=0;
        }
        
        //______________________________________________________________________
        //
        //! store with increasing memory
        //______________________________________________________________________
        NODE *store_increasing_memory(NODE *node) noexcept
        {
            YACK_POOL_CHECK(node);
            assert(this->memory_is_increasing());
            assert(this->owns(node)==false);
            if(size<=0)
            {
                assert(NULL==head);
                head = node;
                goto RETURN;
            }
            else
            {
                assert(NULL!=head);
                if(node<head)
                {
                    node->next = head;
                    head       = node;
                    goto RETURN;
                }
                else
                {
                    NODE *prev = head;
                    NODE *next = prev->next;
                    assert(node>prev);
                    while(next)
                    {
                        if(node<next)
                            break;
                        prev = next;
                        next = next->next;
                    }
                    assert(prev);
                    prev->next = node;
                    node->next = next;
                    goto RETURN;
                }
            }
            
        RETURN:
            this->increase();
            return node;
        }
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(pool_of);
        
    };
    
}

#endif
