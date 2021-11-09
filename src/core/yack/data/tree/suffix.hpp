
//! \file

#ifndef YACK_DATA_SUFFIX_TREE_INCLUDED
#define YACK_DATA_SUFFIX_TREE_INCLUDED 1

#include "yack/container/collection.hpp"
#include "yack/container/releasable.hpp"
#include "yack/data/tree/node.hpp"
#include "yack/data/list/ordered.hpp"

namespace yack
{

    template <typename T, typename CODE>
    class suffix_tree : public collection, public releasable
    {
    public:
        YACK_DECL_ARGS(T,type);
        typedef tree_node<T,CODE>             node_type;
        typedef typename node_type::list_type node_list;
        typedef typename node_type::pool_type node_pool;
        typedef typename node_type::vkey_type vkey_type;

        typedef tree_knot<T,node_type>        knot_type;
        typedef typename knot_type::list_type knot_list;
        typedef typename knot_type::pool_type knot_pool;

        inline explicit suffix_tree() throw() :
        root(),
        data(),
        repo(),
        pool()
        {
        }

        inline virtual ~suffix_tree() throw()
        {
        }
        

        virtual size_t size() const throw() { return data.size; }
        virtual void   free() throw()
        {
            purge(root);
            while(data.size)
                pool.store( data.pop_back()->free() );
        }

        virtual void release() throw()
        {
            root.release();
            data.release();
            repo.release();
            pool.release();
        }



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
                    assert(scan); assert(code==scan->code); assert(scan->from==node);
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
                    //----------------------------------------------------------
                    // found a way
                    //----------------------------------------------------------
                    assert(scan); assert(code==scan->code);
                    node=scan;
                }
                else
                {
                    //----------------------------------------------------------
                    // create a new way
                    //----------------------------------------------------------
                    const node_type *from = node;
                    node = (repo.size>0) ? repo.query()->reset(code) : new node_type(code);
                    if(scan)
                        curr->insert_after(scan,node);
                    else
                        curr->push_front(node);
                    node->from = from;
                }
                curr=&(node->chld);
            }

            assert(node!=NULL);
            if(NULL == node->knot)
            {
                knot_type *knot = (pool.size>0) ? pool.query() : new knot_type();
                try { knot->make(args); } catch(...) { pool.store(knot); throw;}
                knot->node = node;
                node->knot = data.push_back(knot);
                return true;
            }
            else
            {
                // arleady inserted!!
                return false;
            }
        }

        const knot_type *head() const throw() { return data.head; }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(suffix_tree);
        node_list root;
        knot_list data;
        node_pool repo;
        knot_pool pool;

        void purge(node_list &nodes) throw()
        {
            while(nodes.size>0)
            {
                purge(nodes.tail->chld);
                repo.store( nodes.pop_back() )->knot = 0;
            }
        }
        
    };

}

#endif
