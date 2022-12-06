
//! \file

#ifndef YACK_DATA_SMALL_SET_INCLUDED
#define YACK_DATA_SMALL_SET_INCLUDED 1

#include "yack/data/small/bank.hpp"
#include "yack/data/small/list.hpp"
#include "yack/data/nodes-comparison.hpp"
#include "yack/data/list/sort.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! set of items with shared bank for memory I/O
    //
    //__________________________________________________________________________
    template <typename T>
    class small_set
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);                         //!< aliases
        typedef small_node<T>               node_type;  //!< alias
        typedef small_bank<T>               bank_type;  //!< alias
        typedef small_list<T>               list_type;  //!< alias
        typedef typename bank_type::pointer bank_ptr;   //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup with a shared bank of items
        explicit small_set(const bank_ptr &bptr) throw() :
        deposit(bptr),
        content() {}

        //! cleanup, returning used items
        virtual ~small_set() throw() { free(); }

        //! copy content, sharing the same bank
        inline   small_set(const small_set &other) :
        deposit(other.deposit),
        content()
        {
            for(const node_type *node=other->head;node;node=node->next)
                content.push_back( get( **node ) );
        }

        //! assign by copy/swap
        inline small_set & operator=(const small_set &other)
        {
            small_set temp(other);
            content.swap_with(temp.content);
            return *this;
        }

        //______________________________________________________________________
        //
        // access methods
        //______________________________________________________________________
        inline const list_type * operator->() const throw() { return &content; } //!< access
        inline const list_type & operator* () const throw() { return  content; } //!< access

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! force push back
        inline void add(param_type args) { content.push_back( get(args) ); }

        //! force push front
        inline void pre(param_type args) { content.push_front( get(args) ); }

        //! force push back with any argument for constructor
        template <typename U> inline
        type & add(const U &args, const transmogrify_t &) {
            return **content.push_back( get(args) );
        }

        //! return content to the bank
        inline void free() throw() { deposit->collect(content); }

        //! test ownership
        inline bool contains(param_type args) const
        {
            return NULL != content.whose(args);
        }

        //! check overlaps with an other set
        inline bool overlaps(const small_set &other) const
        {
            for(const node_type *node=other->head;node;node=node->next)
            {
                if(contains(**node)) return true;
            }
            return false;
        }
        
        //! compact current set if needed
        inline void compact()
        {
            list_type temp;
            while(content.size)
            {
                if(temp.whose(**content.head))
                    deposit->store( content.pop_front() );
                else
                    temp.push_back( content.pop_front() );
            }
            content.swap_with(temp);
        }


        //! sort data thru sorting of nodes
        template <typename FUNC> inline
        void sort(FUNC &comparison)
        {
            nodes_comparison<node_type,FUNC> call = { comparison };
            merge_list_of<node_type>::sort(content,call);
        }


        //! insert single args
        inline bool insert(param_type args) {
            if(content.whose(args)) return false; else {  add(args); return true; }
        }

        //! ensure single args
        inline void include(param_type args) {
            if(!content.whose(args)) add(args);
        }

        //! syntax helper
        inline small_set & operator +=(param_type args) { include(args); return *this; }

        //! syntax helper
        inline small_set & operator <<(param_type args) { include(args); return *this; }

        //! include another set
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

        
        
        //! get access to memory I/O
        inline const bank_ptr & io() const throw() { return deposit; }

        
        
    private:
        bank_ptr  deposit;
        list_type content;


        template <typename U>
        inline node_type *get(const U &args) {
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

