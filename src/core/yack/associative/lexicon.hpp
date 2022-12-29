
//! \file

#ifndef YACK_ASSOCIATIVE_LEXICON_INCLUDED
#define YACK_ASSOCIATIVE_LEXICON_INCLUDED 1

#include "yack/data/suffix/tree.hpp"
#include "yack/container/iterator/linked.hpp"
#include "yack/associative/be-key.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! lexicon of integral type
    //
    //__________________________________________________________________________
    template <typename T>
    class lexicon : public kernel::suffix_tree<T,uint8_t>
    {
    public:
        typedef kernel::suffix_tree<T,uint8_t> self_type;  //!< alias
        typedef be_key<T>                      bkey_type;  //!< alias
        typedef typename self_type::knot_type  knot_type;  //!< alias
        typedef typename self_type::type       type;       //!< alias
        typedef typename self_type::const_type const_type; //!< alias
        typedef typename self_type::param_type param_type; //!< alias

        //! alias, const only
        typedef iterating::linked<const_type,const knot_type,iterating::forward> const_iterator;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit lexicon() throw() : self_type() {} //!< setup empty
        inline virtual ~lexicon() throw() {}               //!< cleanup
        inline lexicon(const lexicon &_) : self_type(_) {} //!< hard copy

        //! ensure presence of args
        inline void ensure(param_type args)
        {
            const bkey_type key(args);
            (void) self_type::insert(args,key.begin(),key.measure());
        }

        //! insert args
        inline bool insert(param_type args)
        {
            const bkey_type key(args);
            return self_type::insert(args,key.begin(),key.measure());
        }

        //! search if registered
        bool search(param_type args) const throw()
        {
            const bkey_type key(args);
            return self_type::search(key.begin(),key.measure());
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        const_iterator begin() const throw() { return (**this).head; } //!< forward iterator begin
        const_iterator end()   const throw() { return NULL; }          //!< forward itetator end

    private:
        YACK_DISABLE_ASSIGN(lexicon);
    };

}

#endif
