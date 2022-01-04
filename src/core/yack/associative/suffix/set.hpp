//! \file

#ifndef YACK_ASSOCIATIVE_SUFFIX_SET_INCLUDED
#define YACK_ASSOCIATIVE_SUFFIX_SET_INCLUDED 1

#include "yack/associative/suffix/compound.hpp"
#include "yack/associative/registry.hpp"

namespace yack
{
    namespace kernel
    {
        extern const char suffix_set_category[]; //!< "suffix_set"
    }

    //__________________________________________________________________________
    //
    //
    //! suffix_set (key,type), key is not stored but used as path
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
        inline explicit suffix_set() throw() : base_type() {}            //!< setup empty
        inline virtual ~suffix_set() throw() {}                          //!< cleanup
        inline          suffix_set(const suffix_set &other) :
        collection(), base_type(other) {} //!< hard copy

        //! assign by copy/swap, keep resources
        inline suffix_set & operator=(const suffix_set &other)
        {
            if(&other != this)
            {
                suffix_set temp(other);
                this->exchange_content_with(temp);
            }
            return *this;
        }
        
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

        //! category for container
        virtual const char *category() const throw() { return kernel::suffix_set_category; }

        //! output to std::ostream
        friend inline std::ostream & operator<<(std::ostream &os, const suffix_set &s)
        {
            return base_type::template display(os, (*s.tree).head );
        }

        //! output to ios::ostream
        friend inline ios::ostream & operator<<(ios::ostream &os, const suffix_set &s)
        {
            return base_type::template display(os, (*s.tree).head );
        }



    };
}

#endif
