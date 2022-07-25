
//! \file

#ifndef YACK_NETWORK_SORT_INCLUDED
#define YACK_NETWORK_SORT_INCLUDED 1

#include "yack/sort/network/sort2.hpp"
#include "yack/sort/network/sort3.hpp"
#include "yack/sort/network/sort4.hpp"
#include "yack/sort/network/sort5.hpp"
#include "yack/sort/network/sort6.hpp"
#include "yack/sort/network/sort7.hpp"
#include "yack/sort/network/sort8.hpp"
#include "yack/sort/network/sort9.hpp"
#include "yack/sort/network/sort10.hpp"

#include "yack/singleton.hpp"

namespace yack {
    
#define YACK_NETWORK_SORT_PROC(N,METHOD) case N: s##N.METHOD(arr); return

#define YACK_NETWORK_SORT_INCR(N) YACK_NETWORK_SORT_PROC(N,increasing)
#define YACK_NETWORK_SORT_DECR(N) YACK_NETWORK_SORT_PROC(N,decreasing)

#define YACK_NETWORK_SORT_REP(MACRO) \
case 0: case 1: return;\
MACRO(2); MACRO(3); MACRO(4); MACRO(5); MACRO(6); MACRO(7); MACRO(8);\
MACRO(9); MACRO(10)

    class network_sort : public singleton<network_sort>
    {
    public:
        static const at_exit::longevity life_time = 4000;
        static const char * const       call_sign;


        template <typename ARRAY> inline
        void incr_(ARRAY &arr) const
        {
            const size_t n = arr.size();
            switch(n)
            {
                    YACK_NETWORK_SORT_REP(YACK_NETWORK_SORT_INCR);
            }
        }

        template <typename ARRAY> inline
        void decr_(ARRAY &arr) const
        {
            const size_t n = arr.size();
            switch(n)
            {
                    YACK_NETWORK_SORT_REP(YACK_NETWORK_SORT_DECR);
            }
        }





        const network_sort2  s2;
        const network_sort3  s3;
        const network_sort4  s4;
        const network_sort5  s5;
        const network_sort6  s6;
        const network_sort7  s7;
        const network_sort8  s8;
        const network_sort9  s9;
        const network_sort10 s10;

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(network_sort);
        friend class singleton<network_sort>;
        explicit network_sort() throw();
        virtual ~network_sort() throw();
    };

#if 0
    struct network_sort
    {
        template <typename ARRAY> static inline
        void perform(ARRAY &arr)
        {
            static const network_sort2  s2;
            static const network_sort2  s3;
            static const network_sort10 s10;
            const size_t n = arr.size();
            switch(n)
            {
                case 0: return;
                case 1: return;
                case 2: s2.increasing(arr); return;
                case 3: s3.increasing(arr); return;
                default:
                    ;
            }
        }

    };
#endif
    
}

#endif



