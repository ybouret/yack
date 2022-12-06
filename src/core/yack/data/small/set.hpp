
//! \file

#ifndef YACK_DATA_SMALL_SET_INCLUDED
#define YACK_DATA_SMALL_SET_INCLUDED 1

#include "yack/data/small/bank.hpp"
#include "yack/data/small/list.hpp"

namespace yack
{

    //! small set
    template <typename T>
    class small_set
    {
    public:
        YACK_DECL_ARGS(T,type);
        typedef small_node<T> node_type;
        typedef small_bank<T> bank_type;
        typedef small_list<T> list_type;

        typedef typename bank_type::pointer bank_ptr;

        explicit small_set(const bank_ptr &bptr) throw() : deposit(bptr), content() {}
        virtual ~small_set() throw() {}




    private:
        bank_ptr  deposit;
        list_type content;


        inline node_type *get(const_type &args) {
            if(deposit->size)
            {
                node_type *node = deposit->query();
                try { **node = args; } catch(...) { deposit->store(node); throw; }
                return node;
            }
            else
            {
                return new node_type(args);
            }
        }

    };

}

#endif

