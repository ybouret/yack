//! \file

#ifndef YACK_DATA_CLIST_INCLUDED
#define YACK_DATA_CLIST_INCLUDED 1

#include "yack/data/list.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! base class for circular linked list
    //
    //__________________________________________________________________________
    template <typename NODE>
    class clist_of : public interlinked<NODE>
    {
    public:
        //______________________________________________________________________
        //
        // definitions
        //______________________________________________________________________
        using interlinked<NODE>::size;
        using interlinked<NODE>::head;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit clist_of() noexcept : interlinked<NODE>() {}
        inline virtual ~clist_of() noexcept { assert(0==size); }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! testing ownership
        virtual bool owns(const NODE *other) const noexcept
        {
            const NODE *node = head;
            for(size_t i=size;i>0;--i,node=node->next)
            {
                if(node==other) return true;
            }
            return false;
        }

        //! reverse (using internal regular list)
        virtual void reverse() noexcept
        {
            list_of<NODE> temp;
            while(size) temp.push_back(pop_front());
            while(temp.size) push_back( temp.pop_back() );
        }

        //! push_back, a.k.a before head
        inline NODE *push_back(NODE *node) noexcept
        {
            assert(NULL!=node);
            assert(NULL==node->next);
            assert(NULL==node->prev);
            assert(!owns(node));
            switch(size)
            {
                case 0:  push_first(node);  break;
                case 1:  push_back_second(node); break;
                default: push_back_encore(node); break;
            }
            this->increase();
            return node;
        }


        //! push_front a new node
        inline NODE *push_front(NODE *node) noexcept
        {
            assert(NULL!=node);
            assert(NULL==node->next);
            assert(NULL==node->prev);
            assert(!owns(node));
            switch(size)
            {
                case 0:  push_first(node);         break;
                case 1:  push_front_second(node);  break;
                default: push_front_encore(node);  break;
            }
            this->increase();
            return node;
        }

        //! pop_back node before head
        inline NODE *pop_back() noexcept
        {
            assert(size>0);
            assert(NULL!=head);
            switch(size)
            {
                case 1: return pop_last();
                case 2: return pop_back_two();
                default:
                    break;
            }
            return pop_back_any();
        }

        //! pop_back head
        inline NODE *pop_front() noexcept
        {
            assert(size>0);
            assert(NULL!=head);
            switch(size)
            {
                case 1: return pop_last();
                case 2: return pop_front_two();
                default:
                    break;
            }
            return pop_front_any();
        }

        //! NODE in [1:size], slow
        inline NODE *get(const size_t indx) noexcept
        {
            return (NODE *)get_(indx);
        }

        //! const NODE in [1:size], slow
        inline const NODE *get(const size_t indx) const noexcept
        {
            return get_(indx);
        }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(clist_of);

        inline const NODE *get_(size_t indx) const noexcept
        {
            assert(indx>=1);
            assert(indx<=size);
            const size_t half = size >> 1;
            const NODE  *node = head;
            if(indx<=half)
            {
                while(--indx>0) node=node->next;
            }
            else
            {
                indx = size-(--indx);
                while(indx-- > 0) node=node->prev;
            }
            return node;
        }

        inline void push_first(NODE  *node) noexcept {
            assert(0==size);
            head       = node;
            head->next = node;
            head->prev = node;
        }

        inline void push_back_second(NODE *node) noexcept {
            assert(1==size);
            assert(NULL!=head);
            head->next = head->prev = node;
            node->next = node->prev = head;
        }

        inline void push_back_encore(NODE *node) noexcept
        {
            assert(size>=2);
            NODE *prev = head->prev;
            NODE *next = head;
            prev->next = next->prev = node;
            node->next = next;
            node->prev = prev;
        }

        inline void push_front_second(NODE *node) noexcept {
            push_back_second(node);
            head = node;
        }

        inline void push_front_encore(NODE *node) noexcept
        {
            assert(size>=2);
            push_back_encore(node);
            head       = node;
        }

        inline NODE *pop_last() noexcept
        {
            assert(1==size);
            head->next = head->prev = 0;
            NODE *node = head;
            head = NULL;
            this->decrease();
            return node;
        }

        inline NODE *pop_back_two() noexcept
        {
            assert(2==size);
            NODE *node = head->next;
            head->next = head->prev = 0;
            node->next = node->prev = 0;
            this->decrease();
            return node;
        }

        inline NODE *pop_back_any() noexcept
        {
            NODE *node = head->prev;
            NODE *prev = node->prev;
            NODE *next = head;
            prev->next = next;
            next->prev = prev;
            node->next = node->prev = 0;
            this->decrease();
            return node;
        }

        inline NODE *pop_front_two() noexcept
        {
            head = head->next;
            return pop_back_two();
        }

        inline NODE *pop_front_any() noexcept
        {
            head = head->next;
            return pop_back_any();
        }

    };
}

#endif

