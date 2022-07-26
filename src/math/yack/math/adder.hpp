//! \file

#ifndef YACK_MATH_ADDER_INCLUDED
#define YACK_MATH_ADDER_INCLUDED 1

#include "yack/ordered/heap.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/type/abs.hpp"

namespace yack
{
    namespace math
    {

        namespace core
        {
            //! memory allocator for adder
            typedef  memory::dyadic      adder_allocator;

            //! type comparator for adder
            template <typename T> struct adder_comparator
            {
                //! decreasing absolute value
                inline int operator()(const T &lhs, const T &rhs) const
                {
                    const T L = absolute(lhs);
                    const T R = absolute(rhs);
                    return (L<R) ? 1 : ( (R<L) ? -1 : 0);
                }
            };
        }

        
        //______________________________________________________________________
        //
        //
        //! precise sum using a heap as priority queue
        //
        //______________________________________________________________________
        template <typename T>
        class adder : public heap<T,core::adder_comparator<T>,core::adder_allocator>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef heap<T,core::adder_comparator<T>,core::adder_allocator> self_type;    //!< alias
            typedef typename self_type::mutable_type                        mutable_type; //!< alias
            using self_type::size;
            using self_type::pull;
            using self_type::free;
            using self_type::push;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            inline explicit adder() throw() : self_type() {}
            inline virtual ~adder() throw() {}
            inline explicit adder(const size_t n) : self_type(n,as_capacity) {}

            //__________________________________________________________________
            //
            // main algorithm
            //__________________________________________________________________

            //! query by priority
            inline T query()
            {
                if(size()<=0)
                {
                    return 0;
                }
                else
                {
                    while(size()>1)
                    {
                        mutable_type tmp = pull();
                        tmp += pull();
                        push(tmp);
                    }
                    assert(1==size());
                    return pull();
                }
            }

            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________

            //! range
            template <typename ITERATOR> inline
            T range(ITERATOR curr, size_t n)
            {
                free();
                while(n-- > 0) push(*(curr++));
                return query();
            }

            //! full range of sequence
            template <typename SEQUENCE> inline
            T range(SEQUENCE &seq)
            {
                return range(seq.begin(),seq.size());
            }

            //! C++ arr[1..size]
            template <typename ARRAY> inline
            T tableau(ARRAY &arr)
            {
                free();
                for(size_t i=arr.size();i>0;--i)
                {
                    push( arr[i] );
                }
                return query();
            }

            //! C-style arr[0..n-1]
            template <typename U> inline
            T tableau(const U arr[], const size_t n)
            {
                assert(yack_good(arr,n));
                free();
                for(size_t i=0;i<n;++i) push( arr[i] );
                return query();
            }

            //! dot product with implicit conversions
            template <typename LHS, typename RHS> inline
            T dot(LHS &lhs, RHS &rhs)
            {
                assert(lhs.size()==rhs.size());
                free();
                for(size_t i=lhs.size();i>0;--i)
                {
                    push( lhs[i] * rhs[i] );
                }
                return query();
            }





        private:
            YACK_DISABLE_COPY_AND_ASSIGN(adder);
        };

    }
}

#endif

