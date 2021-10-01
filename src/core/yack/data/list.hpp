
//! \file

#ifndef YACK_DATA_LIST_INCLUDED
#define YACK_DATA_LIST_INCLUDED 1

#include "yack/data/linked.hpp"

namespace yack
{

#define YACK_LIST_CHECK(NODE) \
assert(NULL!=NODE); assert(NULL==(NODE)->next); assert(NULL==(NODE)->prev)

    template <typename NODE>
    class list_of : public linked
    {
    public:
        virtual ~list_of() throw() {}

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
        

        NODE *head;
        NODE *tail;

    protected:
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
    };

#if 0
    template <typename NODE>
    class raw_list_of : public list_of<NODE>
    {
    public:

    private:

    };
#endif

}

#endif

