
//! \file

#ifndef YACK_DATA_POOL_INCLUDED
#define YACK_DATA_POOL_INCLUDED 1

#include "yack/data/linked.hpp"
#include "yack/data/core-pool.hpp"

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
        inline virtual ~pool_of() throw() {} //!< need cleanup before this!
        
        //! setup empty
        inline explicit pool_of() throw() : interlinked<NODE>() {}
        
        //! setup from state
        inline explicit pool_of(core_pool_of<NODE> &io) throw() :
        interlinked<NODE>()
        {
            head         = io.head;
            coerce(size) = io.size;
            io.zero();
        }
        
        
        //______________________________________________________________________
        //
        // interlinked interface
        //______________________________________________________________________
        
        inline virtual bool owns(const NODE *node) const throw()
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
            while(size) tmp.store( query() );
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
            NODE *node =  head;
            head       =  head->next;
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
        //! save state
        //______________________________________________________________________
        inline void save(core_pool_of<NODE> &io) throw()
        {
            io.head = head; head=NULL;
            io.size = size; coerce(size)=0;
        }
        
        //______________________________________________________________________
        //
        //! store with increasing memory
        //______________________________________________________________________
        NODE *store_increasing_memory(NODE *node) throw()
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
