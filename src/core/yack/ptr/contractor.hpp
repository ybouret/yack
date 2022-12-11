//! \file

#ifndef YACK_CONTRACTOR_INCLUDED
#define YACK_CONTRACTOR_INCLUDED 1

#include "yack/type/args.hpp"
#include "yack/type/destruct.hpp"
#include "yack/type/out-of-reach.hpp"

#include <iostream>

namespace yack
{

    template <typename T>
    class contractor
    {
    public:
        YACK_DECL_ARGS(T,type);

        inline  contractor() throw() : amount(0),  item_p(0), base_p(0) {}
        inline ~contractor() throw() { implode(); }

        inline contractor(void *addr, const size_t size) :
        amount(0),
        item_p(0),
        base_p( coerce_cast<mutable_type>(addr) )
        {
            assert( yack_good(addr,size) );
            try {
                while(amount<size) {
                    new (base_p+amount) mutable_type();
                    ++coerce(amount);
                }
                init();
            }
            catch(...) { implode(); throw; }
        }

        template <typename ARGS>
        inline contractor(void *addr, const size_t size, const ARGS &args) :
        amount(0),
        item_p(0),
        base_p( coerce_cast<mutable_type>(addr) )
        {
            assert( yack_good(addr,size) );
            try {
                while(amount<size) {
                    new (base_p+amount) mutable_type(args);
                    ++coerce(amount);
                }
                init();
            }
            catch(...) { implode(); throw; }
        }

        template <typename U>
        inline contractor(void        *addr,
                          const size_t size,
                          const U     *param,
                          const size_t shift) :
        amount(0),
        item_p(0),
        base_p( coerce_cast<mutable_type>(addr) )
        {
            assert( yack_good(addr,size) );
            try {
                while(amount<size) {
                    new (base_p+amount) mutable_type(param);
                    ++coerce(amount);
                    param+=shift;
                }
                init();
            }
            catch(...) { implode(); throw; }
        }

        void implode() throw() {
            assert( yack_good(base_p,amount) );
            while(amount>0)
                destruct( &item_p[coerce(amount)--]);
            base_p         = NULL;
            item_p         = NULL;
        }
        
        inline void swap_with(contractor &other) throw() {
            coerce_cswap(item_p,other.item_p);
            coerce_cswap(base_p,other.base_p);
            coerce_cswap(amount,other.amount);
            
        }
        
        type        * operator()(void)       throw() { return base_p; }
        const_type  * operator()(void) const throw() { return base_p; }

        type *       operator()(const size_t shift)       throw() { assert(shift<amount); return base_p+shift; }
        const_type * operator()(const size_t shift) const throw() { assert(shift<amount); return base_p+shift; }
        
        inline type & operator[](size_t index) throw()
        { assert(index>=1); assert(index<=amount); return item_p[index]; }
        
        inline const_type & operator[](size_t index) const throw()
        { assert(index>=1); assert(index<=amount); return item_p[index]; }
        

        const size_t  amount;
       
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(contractor);
        mutable_type *item_p;
        mutable_type *base_p;
        
        inline void init() throw() { assert(base_p); assert(!item_p); item_p = base_p-1; }
    };


}

#endif

