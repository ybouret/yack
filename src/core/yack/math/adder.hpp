//! \file

#ifndef YACK_MATH_ADDER_INCLUDED
#define YACK_MATH_ADDER_INCLUDED 1

#include "yack/ordered/heap.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/math/api.hpp"
#include "yack/large-object.hpp"

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
                    const typename scalar_for<T>::type L = math::abs_of(lhs);
                    const typename scalar_for<T>::type R = math::abs_of(rhs);
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
        class adder : public large_object, public heap<T,core::adder_comparator<T>,core::adder_allocator>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef heap<T,core::adder_comparator<T>,core::adder_allocator> self_type;    //!< alias
            typedef typename self_type::mutable_type                        mutable_type; //!< alias
            typedef typename self_type::const_type                          const_type;   //!< alias

            using self_type::size;
            using self_type::pull;
            using self_type::free;
            using self_type::push;
            using self_type::resume;
            using self_type::ld;
            using self_type::pop;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            inline explicit adder() throw() : self_type() {}                    //!< default constructor
            inline virtual ~adder() throw() {}                                  //!< destructor
            inline explicit adder(const size_t n) : self_type(n,as_capacity) {} //!< constructor with capacity

            //__________________________________________________________________
            //
            // main algorithm
            //__________________________________________________________________

            //! get by priority
            inline T get()
            {
                if(size()<=0)
                    return 0;
                else
                {
                    while(size()>1) {
                        mutable_type tmp = pull();
                        tmp += pull();
                        ld(tmp);
                    }
                    assert(1==size());
                    return pull();
                }
            }

            //! get median
            inline T median() {
                const size_t n = size();
                switch(n)
                {
                    case 0: return 0;
                    case 1: return pull();
                    default:
                        break;
                }

                if( 0!=(n&1) )
                {
                    for(size_t h = n>>1;h>0;--h) pop();
                    return pull();
                }
                else
                {
                    for(size_t h= (n>>1)-1;h>0;--h) pop();
                    const T x1 = pull();
                    const T x2 = pull();
                    return (x1+x2)/2;
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
                resume(n);
                while(n-- > 0) this->ld(*(curr++));
                return get();
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
                fetch(arr);
                return get();
            }

            //! C-style arr[0..n-1]
            template <typename U> inline
            T tableau(const U arr[], const size_t n)
            {
                legacy(arr,n);
                return get();
            }

            //! dot product with implicit conversions
            template <typename LHS, typename RHS> inline
            T dot(LHS &lhs, RHS &rhs)
            {
                assert(lhs.size()==rhs.size());
                const size_t n = lhs.size();
                resume(n);
                for(size_t i=n;i>0;--i)
                {
                    const_type l = lhs[i];
                    const_type r = rhs[i];
                    const_type p = l*r;
                    (*this) += p;
                }
                return get();
            }

            //! get sum of squares
            template <typename LHS> inline
            T squares(LHS &lhs)
            {
                const size_t n = lhs.size();
                resume(n);
                for(size_t i=n;i>0;--i)
                {
                    const_type l  = lhs[i];
                    const_type p  = l*l;
                    (*this) += p;
                }
                return get();
            }
            

            //! get sum of delta squares
            template <typename LHS, typename RHS> inline
            T squares(LHS &lhs, RHS &rhs)
            {
                assert(lhs.size()==rhs.size());
                const size_t n = lhs.size();
                resume(n);
                for(size_t i=n;i>0;--i)
                {
                    const_type l = lhs[i];
                    const_type r = rhs[i];
                    const_type d = l-r;
                    const_type p = d*d;
                    (*this) += p;
                }
                return get();
            }

            //! get sum of function(lhs)
            template <typename LHS, typename FUNC> inline
            T map_to(LHS &lhs, FUNC &func)
            {
                const size_t n = lhs.size();
                resume(n);
                for(size_t i=n;i>0;--i)
                {
                    const_type l  = func(lhs[i]);
                    (*this) += l;
                }
                return get();
            }

            //! get sum of function(lhs-rhs)
            template <typename LHS, typename RHS, typename FUNC> inline
            T map_to(LHS &lhs, RHS &rhs, FUNC &func)
            {
                assert(lhs.size()==rhs.size());
                const size_t n = lhs.size();
                resume(n);
                for(size_t i=n;i>0;--i)
                {
                    const_type l  = func(lhs[i]-rhs[i]);
                    (*this) += l;
                }
                return get();
            }


            //! sum of a pair
            template <typename U, typename V>
            inline T operator()(U u, V v) const
            {
                return u+v;
            }

            //! sum of a triplet
            template <typename U, typename V, typename W>
            inline T operator()(U u, V v, W w)
            {
                self_type &self = *this;
                resume(3);
                self += u;
                self += v;
                self += w;
                return get();
            }

            //! sum of a quadruplet
            template <typename U, typename V, typename W, typename X>
            inline T operator()(U u, V v, W w, X x)
            {
                self_type &self = *this;
                resume(4);
                self += u;
                self += v;
                self += w;
                self += x;
                return get();
            }

            //! sum of a quintuplet
            template <typename U, typename V, typename W, typename X, typename Y>
            inline T operator()(U u, V v, W w, X x, Y y)
            {
                self_type &self = *this;
                resume(5);
                self += u;
                self += v;
                self += w;
                self += x;
                self += y;
                return get();
            }

            //! sum of a sextuplet
            template <typename U, typename V, typename W, typename X, typename Y, typename Z>
            inline T operator()(U u, V v, W w, X x, Y y, Z z)
            {
                self_type &self = *this;
                resume(6);
                self += u;
                self += v;
                self += w;
                self += x;
                self += y;
                self += z;
                return get();
            }

            //! C-style arr[0..n-1]
            template <typename U> inline
            T median(const U arr[], const size_t n)
            {
                legacy(arr,n);
                return median();
            }

            //! C++ style arr[1..m]
            template <typename ARRAY> inline
            T median(ARRAY &arr)
            {
                fetch(arr); return median();
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(adder);

            //! C++ arr[1..size]
            template <typename ARRAY> inline void fetch(ARRAY &arr)
            {
                const size_t n = arr.size();
                resume(n);
                for(size_t i=n;i>0;--i)
                {
                    ld(arr[i]);
                }
                assert(size()==n);
            }

            //! C arr[0..n-1]
            template <typename U> inline void legacy(const U arr[], const size_t n)
            {
                assert(yack_good(arr,n));
                resume(n);
                for(size_t i=0;i<n;++i)
                {
                    ld(arr[i]);
                }
                assert(size()==n);
            }

        };

    }
}

#endif

