#ifndef YACK_DATA_XLIST_INCLUDED
#define YACK_DATA_XLIST_INCLUDED 1

#include "yack/data/zlinked.hpp"
#include "yack/data/list.hpp"
#include "yack/type/args.hpp"
#include "yack/type/destruct.hpp"

namespace yack
{

    template <typename T>
    class xnode
    {
    public:
        YACK_DECL_ARGS(T,type);


        inline  xnode(param_type args) : next(0), prev(0), data(args) {}
        inline ~xnode() throw() {}

        inline type       & operator*() throw() { return data; }
        inline const_type & operator*() const throw() { return data; }

        xnode *next;
        xnode *prev;

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(xnode);
        type data;

    };

    template <typename T>
    class xlist
    {
    public:
        typedef xnode<T>                       node_type;
        typedef list_of< xnode<T> >            list_type;
        typedef typename node_type::param_type args_type;
        typedef zlinked<node_type>             repo_type;

        inline explicit xlist() throw()  : impl() {}
        inline virtual ~xlist() throw() {}

        
        inline node_type * push_back(args_type args, repo_type &repo)
        {
            node_type *node = repo.zquery();
            try {
                return impl.push_back( new (node) node_type(args) );
            }
            catch(...)
            {
                repo.zstore(node);
                throw;
            }
        }

        inline node_type * push_front(args_type args, repo_type &repo)
        {
            node_type *node = repo.zquery();
            try {
                return impl.push_front( new (node) node_type(args) );
            }
            catch(...)
            {
                repo.zstore(node);
                throw;
            }
        }

        inline void pop_back(repo_type &repo) throw()
        {
            repo.zstore( destructed(impl.pop_back()) );
        }

        inline void pop_front(repo_type &repo) throw()
        {
            repo.zstore( destructed(impl.pop_front()) );
        }

        inline void back_to(repo_type &repo) throw()
        {
            while(impl.size) pop_back(repo);
        }
        

        list_type &       operator*()       throw() { return impl; }
        const list_type & operator*() const throw() { return impl; }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(xlist);
        list_type impl;
    };

}


#endif

