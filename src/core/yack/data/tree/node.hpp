//! \file

#ifndef YACK_DATA_TREE_NODE_INCLUDED
#define YACK_DATA_TREE_NODE_INCLUDED 1

#include "yack/sequence/vector.hpp"
#include "yack/data/tree/knot.hpp"
#include "yack/signs.hpp"


namespace yack
{
    template <typename,typename> class suffix_tree;

    //__________________________________________________________________________
    //
    //
    //! node within a tree
    //
    //__________________________________________________________________________
    template <typename T, typename CODE>
    class tree_node : public object
    {
    public:
        //______________________________________________________________________
        //
        // types and definition
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);                        //!< aliases
        typedef vector<CODE,memory::pooled> vkey_type; //!< compiled key
        typedef tree_node<T,CODE>           node_type; //!< alias
        typedef tree_knot<T,node_type>      knot_type; //!< alias
        typedef cxx_list_of<node_type>      list_type; //!< alias
        typedef cxx_pool_of<node_type>      pool_type; //!< aluas

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup
        inline explicit tree_node(const CODE c) throw() :
        code(c), next(NULL), prev(NULL), from(NULL), chld(), knot(NULL)
        {
        }

        //! cleanup
        inline virtual ~tree_node() throw() {}

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________



        inline type       & operator*()       throw() { assert(knot); assert(knot->is_alive()); return **knot; } //!< access data
        inline const_type & operator*() const throw() { assert(knot); assert(knot->is_alive()); return **knot; } //!< access data, condt

        //! comparison for ordered_list ops
        static inline sign_type compare(const CODE &code, const node_type *node) throw()
        {
            return __sign::of(code,node->code);
        }

        //! save compiled key
        void encode( vkey_type &vkey ) const
        {
            vkey.free();
            const node_type *node = this;
            while(node)
            {
                vkey.push_back(node->code);
                node=node->from;
            }
            vkey.reverse();
        }
        
        inline bool prunable() const
        {
            if(knot) return false;
            for(const tree_node *child=chld.head;child;child=child->next)
            {
                
            }
            
        }
        
        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const CODE        code; //!< code for this node
        tree_node        *next; //!< for list/pool
        tree_node        *prev; //!< for list
        const tree_node  *from; //!< origin
        list_type         chld; //!< leaves
        knot_type        *knot; //!< NULL:vacant, otherwise alive

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(tree_node);
        friend class suffix_tree<T,CODE>;

        //! change code
        inline tree_node *reset(const CODE new_code) throw() {
            coerce(code) = new_code;
            return this;
        }

    };

}

#endif

