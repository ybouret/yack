
//! \file

#ifndef YACK_DATA_SUFFIX_TREE_INCLUDED
#define YACK_DATA_SUFFIX_TREE_INCLUDED 1

#include "yack/container/collection.hpp"
#include "yack/container/releasable.hpp"
#include "yack/data/suffix/node.hpp"
#include "yack/data/list/ordered.hpp"
#include "yack/ios/ocstream.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! generic suffix tree
    //
    //__________________________________________________________________________
    template <typename T, typename CODE>
    class suffix_tree : public collection, public releasable
    {
    public:
        //______________________________________________________________________
        //
        // types and definition
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);                           //!< aliases
        typedef tree_node<T,CODE>             node_type;  //!< node in the tree
        typedef typename node_type::list_type node_list;  //!< list of nodes
        typedef typename node_type::pool_type node_pool;  //!< pool of nodes
        typedef typename node_type::path_type path_type;  //!< encoded top-down key

        typedef data_knot<T,node_type>        knot_type;  //!< knot of data
        typedef typename knot_type::list_type knot_list;  //!< list of knots
        typedef typename knot_type::pool_type knot_pool;  //!< pool of knots

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup empty
        inline explicit suffix_tree() throw() :
        root(),
        data(),
        repo(),
        pool()
        {
        }

        //! cleanup
        inline virtual ~suffix_tree() throw()
        {
        }
        
        //______________________________________________________________________
        //
        //! collection:size()
        //______________________________________________________________________
        inline  virtual size_t size() const throw() { return data.size; }

        //______________________________________________________________________
        //
        //! releasable:release()
        //______________________________________________________________________
        virtual void release() throw()
        {
            root.release();
            data.release();
            repo.release();
            pool.release();
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! free content, keep nodes and knots
        inline virtual void   free() throw()
        {
            purge(root);
            while(data.size)
                pool.store( data.pop_back()->free() );
        }

        //! available nodes for the tree
        inline size_t cache() const throw() { return repo.size(); }
        
        //______________________________________________________________________
        //
        //! search method
        //______________________________________________________________________
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
                    return NULL;
            }
            assert(node);
            return (NULL!=node->knot) ? &(**node) : NULL;
            
        }

        //______________________________________________________________________
        //
        //! search method, nutable
        //______________________________________________________________________
        template <typename ITERATOR> inline
        type *search(ITERATOR iter, size_t n) throw()
        {
            const suffix_tree &self = *this; return (type *)(self.search(iter,n));
        }

        //______________________________________________________________________
        //
        //! insertion method
        //______________________________________________________________________
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
                    // create a new way, link
                    //----------------------------------------------------------
                    node_type *from = node;
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

        //______________________________________________________________________
        //
        //! remove
        //______________________________________________________________________
        template <typename ITERATOR> inline
        bool remove(ITERATOR iter, size_t n) throw()
        {
            node_type *node = NULL;
            node_list *curr = &root;

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
                    goto WALK;    // continue
                }
                else
                    return false; // not path
            }
            assert(node);


            if(node->knot)
            {
                //--------------------------------------------------------------
                //
                // free and remove knot
                //
                //--------------------------------------------------------------
                pool.store( data.pop(node->knot)->free() );
                node->knot = 0;

                //--------------------------------------------------------------
                //
                // prune from node
                //
                //--------------------------------------------------------------
                prune(node);

                //--------------------------------------------------------------
                //
                // successfully removed
                //
                //--------------------------------------------------------------
                return true;

            }
            else
            {
                //--------------------------------------------------------------
                //
                // empty or already removed path
                //
                //--------------------------------------------------------------
                return false;
            }
        }


        //______________________________________________________________________
        //
        //! first knot of data
        //______________________________________________________________________
        const knot_type *head() const throw() { return data.head; }

        //______________________________________________________________________
        //
        //! last knot of data
        //______________________________________________________________________
        const knot_type *tail() const throw() { return data.tail; }

        //______________________________________________________________________
        //
        //! GraphViz encoding
        //______________________________________________________________________
        template <typename FILENAME>
        inline void gv(const FILENAME &filename) const
        {
            ios::ocstream fp(filename);
            ios::vizible::digraph_init(fp,"G");

            ios::vizible::end(ios::vizible::uuid(fp,this) << " [label=\"root\"]");

            for(const node_type *node=root.head;node;node=node->next) {
                node->gv(fp);
                ios::vizible::end(ios::vizible::arrow(fp,this,node));
            }

            ios::vizible::digraph_quit(fp);
        }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(suffix_tree);
        node_list root;
        knot_list data;
        node_pool repo;
        knot_pool pool;

        //______________________________________________________________________
        //
        //! recursively purge nodes, disarding knots
        //______________________________________________________________________
        inline void purge(node_list &nodes) throw()
        {
            while(nodes.size>0)
            {
                purge(nodes.tail->chld);
                repo.store( nodes.pop_back() )->knot = 0;
            }
        }

        //______________________________________________________________________
        //
        //! prune from a vacant node
        //______________________________________________________________________
        inline void prune(node_type *node) throw()
        {
            assert(NULL!=node);
            assert(NULL==node->knot);

            if(node->chld.size<=0)
            {
                node_type *parent = node->from;
                node->from = NULL;
                if(parent)
                {
                    assert(parent->chld.owns(node));
                    repo.store(parent->chld.pop(node));
                    if(NULL==parent->knot)
                        prune(parent);
                }
                else
                {
                    assert(root.owns(node));
                    repo.store(root.pop(node));
                }
            }
            else
            {
                assert(node->has_content());
            }
        }

        
    };

}

#endif
