//! \file

#ifndef YACK_ASSOCIATIVE_SUFFIX_COMPOUND_INCLUDED
#define YACK_ASSOCIATIVE_SUFFIX_COMPOUND_INCLUDED 1

#include "yack/associative/suffix/key-walker.hpp"
#include "yack/data/suffix/tree.hpp"

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

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~suffix_compound() throw() {}                         //!< cleanup
    protected:
        inline explicit suffix_compound() throw() : ASSOCIATIVE(), walk() {} //!< setup
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(suffix_compound);

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
    public:
        mutable KEY_WALKER walk; //!< convert KEY to path
    protected:
        tree_type          tree; //!< holding data

    };

}

#endif

