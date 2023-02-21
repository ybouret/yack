//! \file

#ifndef YACK_DATA_ACTUAL_POOL_INCLUDED
#define YACK_DATA_ACTUAL_POOL_INCLUDED 1

#include "yack/data/pool/cxx.hpp"
#include "yack/data/pool/sort.hpp"
#include "yack/type/args.hpp"

namespace yack
{
    
    //__________________________________________________________________________
    //
    //
    //! list of C++ data
    //
    /**
     - NODE must be constructed with param_type
     - NODE must have a copy constructor
     - NODE must have a operator*
     */
    //__________________________________________________________________________
    template <typename T, typename NODE>
    class kpool : public cxx_pool_of<NODE>
    {
    public:
        YACK_DECL_ARGS(T,type);               //!< aliases
        typedef cxx_pool_of<NODE> pool_type;  //!< alias
        typedef NODE              node_type;  //!< alias
        using pool_type::size;
        using pool_type::head;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit kpool() noexcept : pool_type() {} //!< setup empty
        inline virtual ~kpool() noexcept {}               //!< cleanup

        //! copy
        inline          kpool(const kpool &_) noexcept : pool_type(_) {}

        //! assign by copy/swap
        inline kpool & operator=(const kpool &_)
        {
            kpool tmp(_);
            this->swap_with(tmp);
            return *this;
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        //! store new node, return content
        inline type & push(param_type args) { return **(this->store( new NODE(args)) ); }

        //! delete head node
        inline void   pop() noexcept { delete this->query(); }

        //! copy head content, delete head, return content
        inline type   pull() { const_type tmp = **head; delete this->query(); return tmp; }

    };
    
}

#endif

