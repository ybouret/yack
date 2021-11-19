
#ifndef YACK_ASSOCIATIVE_SUFFIX_SET_INCLUDED
#define YACK_ASSOCIATIVE_SUFFIX_SET_INCLUDED 1

#include "yack/associative/suffix/compound.hpp"
#include "yack/associative/registry.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! suffix_sey (key,type), key is not stored but used as path
    //
    //__________________________________________________________________________
    template <typename KEY,
    typename T,
    typename KEY_WALKER = key_walker<KEY>
    >
    class suffix_set : public suffix_compound<KEY,T,KEY_WALKER, registry<KEY,T> >
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);                                                //!< aliases
        YACK_DECL_ARGS(KEY,key_type);                                          //!< aliases
        typedef suffix_compound<KEY,T,KEY_WALKER, registry<KEY,T> > base_type; //!< alias
        using base_type::tree;
        using base_type::walk;
        
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit suffix_set() throw() : base_type() {} //!< setup empty
        inline virtual ~suffix_set() throw() {}               //!< cleanup
        
        //______________________________________________________________________
        //
        // glossary interface
        //______________________________________________________________________
        
        //! insert by key to path walking
        virtual bool insert(param_type args)
        {
            size_t         len = 0;
            param_key_type key = args.key();
            const uint8_t *ptr = walk(key,len);
            return tree.insert(args,ptr,len);
        }
        
        
        
        
        
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(suffix_set);
    };
}

#endif
