//! \file

#ifndef YACK_CONTRACTOR_INCLUDED
#define YACK_CONTRACTOR_INCLUDED 1

#include "yack/type/args.hpp"
#include "yack/type/destruct.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/memory/aligned.hpp"

#include <iostream>

namespace yack
{

    //! initialize default fields
#define YACK_MEM_CTOR_FIELDS() amount(0), item_p(0),  base_p( coerce_cast<mutable_type>(addr) )

    //! prolog for constructor code
#define YACK_MEM_CTOR_PROLOG() \
/**/   assert( yack_good(addr,size) );\
/**/   try {\
/**/     while(amount<size) {\
/**/       new (base_p+amount)

    //! epilog for constructor code
#define YACK_MEM_CTOR_EPILOG() \
/**/       ++coerce(amount); } \
/**/    init(); }\
/**/  catch(...) { resign(); throw; }


    //__________________________________________________________________________
    //
    //
    //! helper for resources acquisition in two-steps
    //
    //__________________________________________________________________________
    template <typename T>
    class contractor
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type); //!< aliases
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        
        //! default, empty
        inline  contractor() noexcept : amount(0),  item_p(0), base_p(0) {}
        
        //! clean up
        inline ~contractor() noexcept { resign(); }
        
        
        
        //! default construction of 'size' items from given 'addr'
        inline contractor(void *addr, const size_t size) :
        YACK_MEM_CTOR_FIELDS()
        {
            YACK_MEM_CTOR_PROLOG()
            mutable_type();
            YACK_MEM_CTOR_EPILOG()
        }
        
        //! construction of 'size' items from given 'addr' with single 'args'
        template <typename ARGS>
        inline contractor(void *addr, const size_t size, const ARGS &args) :
        YACK_MEM_CTOR_FIELDS()
        {
            YACK_MEM_CTOR_PROLOG()
            mutable_type(args);
            YACK_MEM_CTOR_EPILOG()
        }
        
        //! addr[i] = new type(param[i*shift])
        template <typename U> inline
        contractor(void        *addr,
                   const size_t size,
                   U           *param,
                   const size_t shift) :
        YACK_MEM_CTOR_FIELDS()
        {
            YACK_MEM_CTOR_PROLOG()
            mutable_type(param,shift);
            param+=shift;
            YACK_MEM_CTOR_EPILOG()
        }
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        
        //! destruct content of linear memory
        inline void resign() noexcept {
            assert( yack_good(base_p,amount) );
            while(amount>0)
                destruct( &item_p[coerce(amount)--]);
            base_p         = NULL;
            item_p         = NULL;
        }
        
        //! no-throw swap
        inline void swap_with(contractor &other) noexcept {
            coerce_cswap(item_p,other.item_p);
            coerce_cswap(base_p,other.base_p);
            coerce_cswap(amount,other.amount);
            
        }
        
        //! access first item
        type        * operator()(void)       noexcept { return base_p; }
        
        //! access first item, const
        const_type  * operator()(void) const noexcept { return base_p; }
        
        //! access first item + shift
        type *       operator()(const size_t shift)       noexcept { assert(shift<amount); return base_p+shift; }
        
        //! access first item + shift, const
        const_type * operator()(const size_t shift) const noexcept { assert(shift<amount); return base_p+shift; }
        
        //! access in 1:amount
        inline type & operator[](size_t index) noexcept
        { assert(index>=1); assert(index<=amount); return item_p[index]; }
        
        //! access in 1:amount
        inline const_type & operator[](size_t index) const noexcept
        { assert(base_p); assert(item_p); assert(index>=1); assert(index<=amount); return item_p[index]; }
        
        
        
        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const size_t  amount; //!< current amount of managed items
        
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(contractor);
        mutable_type *item_p;
        mutable_type *base_p;
        
        inline void   init() noexcept { assert(base_p); assert(!item_p); item_p = base_p-1; }
        inline size_t reserved() const noexcept {
            return memory::aligned::package(sizeof(T)*amount);
        }
        
    };


}

#endif

