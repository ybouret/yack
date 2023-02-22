//! \file

#ifndef YACK_CAMEO_ADD_INCLUDED
#define YACK_CAMEO_ADD_INCLUDED 1

#include "yack/cameo/def.hpp"
#include "yack/ordered/pqueue.hpp"
#include "yack/math/api.hpp"

namespace yack
{
    namespace cameo
    {
        //______________________________________________________________________
        //
        //
        //! type comparator for add
        //
        //______________________________________________________________________
        template <typename T> struct add_comparator
        {
            //! decreasing absolute value
            inline int operator()(const T &lhs, const T &rhs) const
            {
                const typename scalar_for<T>::type L = math::abs_of(lhs);
                const typename scalar_for<T>::type R = math::abs_of(rhs);
                return (L<R) ? 1 : ( (R<L) ? -1 : 0);
            }
        };

        //______________________________________________________________________
        //
        //
        //! addition with priority queue
        //
        //______________________________________________________________________
        template <typename T>
        class add : public object_type, public pqueue<T, add_comparator<T>, allocator_type>
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            YACK_DECL_ARGS(T,type);
            typedef pqueue<T, add_comparator<T>,allocator_type> self_type;

            using self_type::size;
            using self_type::pull;
            using self_type::push_;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            inline explicit add() noexcept : object_type(), self_type() {}
            inline virtual ~add() noexcept {}

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            inline T sum() {
                switch(size())
                {
                    case 0:  return 0;
                    case 1:  return pull();
                    default: break;
                }
                assert( size() > 1 );
            REDUCE:
                    const_type lhs = pull(); assert(size()>=1);
                    const_type rhs = pull();
                    const_type tmp = lhs+rhs;
                    if(size()<=0) return tmp;
                    push_(tmp);
                goto REDUCE;
            }

            template <typename ITERATOR> inline void toto()
            {
            }




        private:
            YACK_DISABLE_COPY_AND_ASSIGN(add);
        };
    }
}

#endif


