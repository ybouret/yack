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
            typedef  memory::dyadic      adder_allocator;
            template <typename T> struct adder_comparator
            {
                inline int operator()(const T &lhs, const T &rhs) const
                {
                    const T L = absolute(lhs);
                    const T R = absolute(rhs);
                    return (L<R) ? 1 : ( (R<L) ? -1 : 0);
                }
            };
        }

        template <typename T>
        class adder : public heap<T,core::adder_comparator<T>,core::adder_allocator>
        {
        public:
            typedef heap<T,core::adder_comparator<T>,core::adder_allocator> self_type;
            typedef typename self_type::mutable_type mutable_type;
            using self_type::size;
            using self_type::pull;
            using self_type::free;
            using self_type::push;

            inline explicit adder() throw() : self_type() {}
            inline virtual ~adder() throw() {}
            inline explicit adder(const size_t n) : self_type(n,as_capacity) {}

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




        private:
            YACK_DISABLE_COPY_AND_ASSIGN(adder);
        };

    }
}

#endif

