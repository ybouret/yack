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

        inline  contractor() throw() : arr(0), num(0) {}
        inline ~contractor() throw() { implode(); }

        inline contractor(void *addr, const size_t size) :
        arr( coerce_cast<mutable_type>(addr) ),
        num( 0 )
        {
            assert( yack_good(addr,size) );
            try {
                while(num<size) {
                    new (arr+num) mutable_type();
                    ++num;
                }
            }
            catch(...) { implode(); throw; }
        }

        template <typename ARGS>
        inline contractor(void *addr, const size_t size, const ARGS &args) :
        arr( coerce_cast<mutable_type>(addr) ),
        num( 0 )
        {
            assert( yack_good(addr,size) );
            try {
                while(num<size) {
                    new (arr+num) mutable_type(args);
                    ++num;
                }
            }
            catch(...) { implode(); throw; }
        }

        template <typename U>
        inline contractor(void        *addr,
                          const size_t size,
                          const U     *param,
                          const size_t shift) :
        arr( coerce_cast<mutable_type>(addr) ),
        num( 0 )
        {
            assert( yack_good(addr,size) );
            try {
                while(num<size) {
                    new (arr+num) mutable_type(param);
                    ++num;
                    param+=shift;
                }
            }
            catch(...) { implode(); throw; }
        }



        void implode() throw() {
            assert( yack_good(arr,num) );
            while(num-- > 0 ) {
                destruct( &arr[num] );
            }
            arr = NULL;
            num = 0;
        }

        type        * operator()(void)       throw() { return arr; }
        const_type  * operator()(void) const throw() { return arr; }

        type * operator()(const size_t shift) throw() { assert(shift<num); return arr+shift; }
        const_type * operator()(const size_t shift) const throw() { assert(shift<num); return arr+shift; }



    private:
        YACK_DISABLE_COPY_AND_ASSIGN(contractor);
        mutable_type *arr;
        size_t        num;



    };


}

#endif

