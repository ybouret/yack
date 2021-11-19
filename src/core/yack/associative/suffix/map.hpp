

#ifndef YACK_ASSOCIATIVE_SUFFIX_MAP_INCLUDED
#define YACK_ASSOCIATIVE_SUFFIX_MAP_INCLUDED 1

#include "yack/associative/suffix/compound.hpp"
#include "yack/associative/glossary.hpp"
namespace yack
{

    template <typename KEY,
    typename T,
    typename KEY_WALKER = key_walker<KEY>
    >
    class suffix_map : public suffix_compound<KEY,T,KEY_WALKER, glossary<KEY,T> >
    {
    public:
        YACK_DECL_ARGS(T,type);
        YACK_DECL_ARGS(KEY,key_type);
        typedef suffix_compound<KEY,T,KEY_WALKER, glossary<KEY,T> > base_type;
        using base_type::tree;
        using base_type::walk;


        inline explicit suffix_map() throw() : base_type() {}
        inline virtual ~suffix_map() throw() {}

        virtual bool insert(param_key_type key, param_type args)
        {
            size_t         len = 0;
            const uint8_t *ptr = walk(key,len);
            return tree.insert(args,ptr,len);
        }






    private:
        YACK_DISABLE_COPY_AND_ASSIGN(suffix_map);
    };


}

#endif
