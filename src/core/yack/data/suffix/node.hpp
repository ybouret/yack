//! \file

#ifndef YACK_DATA_TREE_NODE_INCLUDED
#define YACK_DATA_TREE_NODE_INCLUDED 1

#include "yack/sequence/vector.hpp"
#include "yack/data/suffix/knot.hpp"
#include "yack/signs.hpp"
#include "yack/ios/gv/vizible.hpp"
#include "yack/ios/ostream.hpp"


namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! forward declaration
    //
    //__________________________________________________________________________
    template <typename,typename> class suffix_tree;

    //__________________________________________________________________________
    //
    //
    //! compile tree_key = path to data
    //
    //__________________________________________________________________________
    template <typename CODE>
    class tree_path : public vector<CODE,memory::pooled>
    {
    public:
        typedef vector<CODE,memory::pooled> base_type; //!< alias

        inline  explicit tree_path() throw() : collection(), base_type() {}   //!< setup
        inline  virtual ~tree_path() throw() {}                               //!< cleanup
        inline  tree_path(const tree_path &_) : collection(), base_type(_) {} //!< hard copy

    private:
        YACK_DISABLE_ASSIGN(tree_path);
    };


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
        typedef tree_path<CODE>             path_type; //!< compiled path
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

        //______________________________________________________________________
        //
        //! save compiled key
        //______________________________________________________________________
        void encode( path_type &path ) const
        {
            path.free();
            const node_type *node = this;
            while(node)
            {
                path.push_back(node->code);
                node=node->from;
            }
            path.reverse();
        }

        //______________________________________________________________________
        //
        //! recursive cheking of load
        //______________________________________________________________________
        inline bool is_loaded() const
        {
            if(knot) return true;
            for(const tree_node *child=chld.head;child;child=child->next)
            {
                if(child->is_loaded()) return true;
            }
            return false;
        }


        //______________________________________________________________________
        //
        //! grapviz encoding
        //______________________________________________________________________
        inline ios::ostream &gv(ios::ostream &fp) const
        {
            ios::vizible::uuid(fp,this) << '[';
            if(knot)
            {
                fp << "shape=rectangle,style=bold";
            }
            else
            {
                fp << "shape=ellipse";
            }
            fp << ",label=\"";
            ios::vizible::make_label(fp,&code,sizeof(CODE));
            fp << "\"";
            ios::vizible::end(fp << ']');
            for(const tree_node *child=chld.head;child;child=child->next)
            {
                child->gv(fp);
                ios::vizible::end(ios::vizible::arrow(fp,this,child));
            }
            return fp;
        }


        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const CODE        code; //!< code for this node
        tree_node        *next; //!< for list/pool
        tree_node        *prev; //!< for list
        tree_node        *from; //!< origin
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

