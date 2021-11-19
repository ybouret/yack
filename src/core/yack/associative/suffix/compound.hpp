//! \file

#ifndef YACK_ASSOCIATIVE_SUFFIX_COMPOUND_INCLUDED
#define YACK_ASSOCIATIVE_SUFFIX_COMPOUND_INCLUDED 1

#include "yack/associative/suffix/key-walker.hpp"
#include "yack/data/suffix/tree.hpp"

namespace yack
{

    template <
    typename KEY,
    typename T,
    typename KEY_WALKER,
    typename ASSOCIATIVE>
    class suffix_compound : public ASSOCIATIVE
    {
    public:
        YACK_DECL_ARGS_(T,type);
        YACK_DECL_ARGS(KEY,key_type);

        typedef kernel::suffix_tree<T,uint8_t> tree_type;

        inline virtual ~suffix_compound() throw() {}

        inline virtual const_type *search(param_key_type key) const throw()
        {
            size_t         len = 0;
            const uint8_t *ptr = walk(key,len);
            return tree.search(ptr,len);
        }




    protected:
        inline explicit suffix_compound() throw() : ASSOCIATIVE(), walk() {}
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(suffix_compound);


    public:
        mutable KEY_WALKER walk;
    protected:
        tree_type          tree;

    };

}

#endif

