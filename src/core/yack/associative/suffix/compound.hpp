//! \file

#ifndef YACK_ASSOCIATIVE_SUFFIX_COMPOUND_INCLUDED
#define YACK_ASSOCIATIVE_SUFFIX_COMPOUND_INCLUDED 1

#include "yack/associative/suffix/key-walker.hpp"
#include "yack/data/suffix/tree.hpp"
#include "yack/container/iterator/linked.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! base class to use tree for set/map
    //
    //__________________________________________________________________________
    template <
    typename KEY,
    typename T,
    typename KEY_WALKER,
    typename ASSOCIATIVE>
    class suffix_compound : public ASSOCIATIVE
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS_(T,type);                           //!< aliases
        YACK_DECL_ARGS(KEY,key_type);                      //!< aliases
        typedef kernel::suffix_tree<T,uint8_t> tree_type;  //!< alias
        typedef typename tree_type::knot_type  knot_type;  //!< alias

        //______________________________________________________________________
        //
        // ASSOCIATIVE interface
        //______________________________________________________________________
        //! search method
        inline virtual const_type *search(param_key_type key) const throw()
        {
            size_t         len = 0;
            const uint8_t *ptr = walk(key,len);
            return tree.search(ptr,len);
        }

        //! remove method
        inline virtual bool remove(param_key_type key) throw()
        {
            size_t         len = 0;
            const uint8_t *ptr = walk(key,len);
            return tree.remove(ptr,len);
        }

        //! size interface
        inline virtual size_t size() const throw() { return (*tree).size; }

        //______________________________________________________________________
        //
        // specific methods
        //______________________________________________________________________

        //! get tree structure
        inline const tree_type &get_tree() const throw() { return tree; }

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~suffix_compound() throw() {}                                 //!< cleanup
    protected:
        inline explicit suffix_compound() throw() : ASSOCIATIVE(), walk(), tree() {} //!< setup

        //! copy tree 
        inline suffix_compound(const suffix_compound &other) :
        ASSOCIATIVE(), walk(), tree(other.tree)
        {
        }

    private:
        YACK_DISABLE_ASSIGN(suffix_compound);

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
    public:
        mutable KEY_WALKER walk; //!< convert KEY to path
    protected:
        tree_type          tree; //!< holding data

    public:
        //______________________________________________________________________
        //
        // iterators
        //______________________________________________________________________
        typedef iterating::linked<const_type,const knot_type,iterating::forward> const_iterator; //!< forward const iterator
        const_iterator begin() const throw() { return (*tree).head; }                            //!< forward const begin
        const_iterator end()   const throw() { return NULL;         }                            //!< forward const end
      
    };

}

#endif

