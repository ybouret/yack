
//! \file

#ifndef YACK_DATA_SUFFIX_TREE_INCLUDED
#define YACK_DATA_SUFFIX_TREE_INCLUDED 1

#include "yack/data/tree/node.hpp"
#include "yack/data/list/ordered.hpp"

namespace yack
{

    template <typename T, typename CODE>
    class suffix_tree
    {
    public:
        YACK_DECL_ARGS(T,type);
        typedef tree_node<T,CODE>             node_type;
        typedef typename node_type::list_type node_list;
        typedef typename node_type::pool_type node_pool;

        typedef tree_knot<T,node_type>        knot_type;
        typedef typename knot_type::list_type knot_list;
        typedef typename knot_type::pool_type knot_pool;

        inline explicit suffix_tree() throw() :
        root(),
        data(),
        zn(),
        zk()
        {
        }

        inline virtual ~suffix_tree() throw()
        {
        }
        

        virtual size_t size() const throw() { return data.size; }



        template <typename ITERATOR> inline
        const_type *search(ITERATOR iter, size_t n) const throw()
        {
            const node_type *node = NULL;
            const node_list *curr = &root;

            //------------------------------------------------------------------
            //
            // walk down the tree
            //
            //------------------------------------------------------------------
        WALK:
            if(n-- > 0)
            {
                //--------------------------------------------------------------
                // get code to look for
                //--------------------------------------------------------------
                const CODE code = *(iter++);
                node_type *scan = NULL;
                if( ordered_list::search(*curr,code,node_type::compare,scan) )
                {
                    assert(scan); assert(code==scan->code);
                    node=scan;
                    curr=&(node->chld);
                    goto WALK;
                }
                else
                {
                    return NULL;
                }
            }
            assert(node);
            return (NULL!=node->knot) ? &(**node) : NULL;
            
        }

        template <typename ITERATOR> inline
        bool insert(param_type args, ITERATOR iter, size_t n)
        {
            assert(n>0);
            node_type *node = NULL;
            node_list *curr = &root;

            //------------------------------------------------------------------
            //
            // walk down the tree
            //
            //------------------------------------------------------------------
            while(n--)
            {
                //--------------------------------------------------------------
                // get code to look for
                //--------------------------------------------------------------
                const CODE code = *(iter++);
                node_type *scan = NULL;
                if( ordered_list::search(*curr,code,node_type::compare,scan) )
                {
                    assert(scan); assert(code==scan->code);
                    node=scan;
                }
                else
                {
                    node = (zn.size>0) ? zn.query()->reset(code) : new node_type(code);
                    if(scan)
                        curr->insert_after(scan,node);
                    else
                        curr->push_front(node);
                }
                curr=&(node->chld);
            }

            assert(node!=NULL);
            if(NULL == node->knot)
            {
                knot_type *knot = (zk.size>0) ? zk.query() : new knot_type();
                try { knot->make(args); } catch(...) { zk.store(knot); throw;}
                node->knot = data.push_back(knot);
                return true;
            }
            else
                return false;
        }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(suffix_tree);
        node_list root;
        knot_list data;
        node_pool zn;
        knot_pool zk;
        
    };

}

#endif
