

#ifndef YACK_ASSOCIATIVE_SUFFIX_MAP_INCLUDED
#define YACK_ASSOCIATIVE_SUFFIX_MAP_INCLUDED 1

#include "yack/associative/suffix/compound.hpp"
#include "yack/associative/glossary.hpp"
namespace yack
{

    namespace kernel
    {
        extern const char suffix_map_category[];
    }

    //__________________________________________________________________________
    //
    //
    //! suffix_map (key,type), key is not stored but used as path
    //
    //__________________________________________________________________________
    template <typename KEY,
    typename T,
    typename KEY_WALKER = key_walker<KEY>
    >
    class suffix_map : public suffix_compound<KEY,T,KEY_WALKER, glossary<KEY,T> >
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);                                                //!< aliases
        YACK_DECL_ARGS(KEY,key_type);                                          //!< aliases
        typedef suffix_compound<KEY,T,KEY_WALKER, glossary<KEY,T> > base_type; //!< alias
        using base_type::tree;
        using base_type::walk;


        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit suffix_map() throw() : base_type() {} //!< setup empty
        inline virtual ~suffix_map() throw() {}               //!< cleanup
        inline          suffix_map(const suffix_map &other) :
        collection(),
        base_type(other) {} //!< hard copy

        //______________________________________________________________________
        //
        // glossary interface
        //______________________________________________________________________
        
        //! insert by key to path walking
        virtual bool insert(param_key_type key, param_type args)
        {
            size_t         len = 0;
            const uint8_t *ptr = walk(key,len);
            return tree.insert(args,ptr,len);
        }



        virtual const char *category() const throw() { return kernel::suffix_map_category; }



    private:
        YACK_DISABLE_ASSIGN(suffix_map);
    };


}

#endif
