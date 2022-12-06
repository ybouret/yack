
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

        inline   small_set(const small_set &other) :
        deposit(other.deposit),
        content()
        {
            for(const node_type *node=other->head;node;node=node->next)
                content.push_back( get( **node ) );
        }

        inline small_set & operator=(const small_set &other)
        {
            small_set temp(other);
            content.swap_with(temp.content);
            return *this;
        }


        inline const list_type * operator->() const throw() { return &content; }
        inline const list_type & operator* () const throw() { return  content; }

        //! force push back
        inline void add(param_type args) {
            content.push_back( get(args) );
        }

        //! force push front
        inline void pre(param_type args) {
            content.push_front( get(args) );
        }

        inline void free() throw() {
            while(content.size) deposit->store( content.pop_back() );
        }

        inline bool contains(param_type args) const
        {
            return NULL != content.whose(args);
        }


        //! insert single args
        inline bool insert(param_type args) {
            if(content.whose(args)) return false; else { add(args); return true; }
        }

        //! ensure single args
        inline void include(param_type args) {
            if(!content.whose(args)) add(args);
        }

        //! syntax helper
        inline small_set & operator +=(param_type args) { include(args); return *this; }

        //! syntax helper
        inline small_set & operator <<(param_type args) { include(args); return *this; }

        inline void include(const small_set &other)
        {
            if(this != &other)
            {
                for(const node_type *node=other->head;node;node=node->next)
                    include(**node);
            }
        }

        //! syntax helper
        inline small_set & operator +=(const small_set &other) { include(other); return *this; }

        //! syntax helper
        inline small_set & operator<<(const small_set &other) { include(other); return *this; }

        //! remove FIRST matching args
        inline bool remove(param_type args)
        {
            node_type *node = content.whose(args);
            if(node)
            {
                deposit->store( content.pop(node) );
                return true;
            }
            else
            {
                return false;
            }
        }

        //! exclude all matching node
        inline void exclude(param_type args)
        {
            list_type keep;
            while(content.size)
            {
                if( args == **(content.head) )
                    deposit->store(content.pop_front());
                else
                    keep.push_back(content.pop_front());
            }
            content.swap_with(keep);
        }

        //! syntax helper
        inline small_set & operator-= (param_type args) { exclude(args); return *this; }

        //! exclude matching subset
        inline void exclude(const small_set &other)
        {
            if(this == &other)
            {
                free();
            }
            else
            {
                list_type keep;
                while(content.size)
                {
                    if(other.contains( **(content.head) ) )
                        deposit->store(content.pop_front());
                    else
                        keep.push_back(content.pop_front());
                }
                content.swap_with(keep);
            }
        }

        //! syntax helper
        inline small_set & operator-= (const small_set &other) { exclude(other); return *this; }

        
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

