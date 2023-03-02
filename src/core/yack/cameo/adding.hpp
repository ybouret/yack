
//! \file

#ifndef YACK_CAMEO_ADDING_INCLUDED
#define YACK_CAMEO_ADDING_INCLUDED 1

#include "yack/cameo/def.hpp"
#include "yack/ordered/heap.hpp"
#include "yack/math/api.hpp"

namespace yack
{
    namespace cameo
    {
        //______________________________________________________________________
        //
        //
        //! material to use extended addition
        //
        //______________________________________________________________________
        struct adding
        {
            //__________________________________________________________________
            //
            //! comparator
            //__________________________________________________________________
            template <typename T> struct  comparator
            {
                //______________________________________________________________
                //
                //! decreasing absolute value
                //______________________________________________________________
                inline int operator()(const T& lhs, const T& rhs) const
                {
                    const typename scalar_for<T>::type L = math::abs_of(lhs);
                    const typename scalar_for<T>::type R = math::abs_of(rhs);
                    return (L < R) ? 1 : ((R < L) ? -1 : 0);
                }
            };

            //__________________________________________________________________
            //
            //! API for add/static_add
            //__________________________________________________________________
            template <typename T, typename MEM_BUFFER>
            class proto : public object_type, public heap<T,comparator<T>,MEM_BUFFER>
            {
            public:
                //______________________________________________________________
                //
                // types
                //______________________________________________________________
                YACK_DECL_ARGS(T,type);                             //!< aliases
                typedef heap<T,comparator<T>,MEM_BUFFER> heap_type; //!< alias

                //______________________________________________________________
                //
                // using
                //______________________________________________________________
                using heap_type::size;
                using heap_type::pull;
                using heap_type::push_;
                using heap_type::resume;

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                inline explicit proto() noexcept      : object_type(), heap_type()  {} //!< setup default
                inline explicit proto(const size_t n) : object_type(), heap_type(n) {} //!< setup with possible capacity
                inline virtual ~proto() noexcept                                    {} //!< cleanup

                //______________________________________________________________
                //
                // sum algorithm
                //______________________________________________________________

                //! get the current sum
                inline T sum() {
                    switch (size())
                    {
                        case 0:  return 0;
                        case 1:  return pull();
                        default: break;
                    }
                    assert(size() > 1);
                REDUCE:
                    const_type lhs = pull(); assert(size() >= 1);
                    const_type rhs = pull();
                    const_type tmp = lhs + rhs;
                    if (size() <= 0) return tmp;
                    push_(tmp);
                    goto REDUCE;
                }

                //______________________________________________________________
                //
                // helpers
                //______________________________________________________________

                //! sum of a range
                template <typename ITERATOR>
                inline T range(ITERATOR i, size_t n)
                {
                    resume(n);
                    while (n-- > 0) push_(*(i++));
                    return sum();
                }

                //! sum of a sequence
                template <typename SEQUENCE>
                inline T range(SEQUENCE& seq)
                {
                    return range(seq.begin(), seq.size());
                }

                //! sum of a C++ array
                template <typename ARR>
                inline T tableau(ARR& arr)
                {
                    size_t n = arr.size();
                    resume(n);
                    while (n > 0) push_(arr[n--]);
                    return sum();
                }

                //! sum of a C array
                template <typename U>
                inline T tableau(U arr[], const size_t num)
                {
                    assert(yack_good(arr, num));
                    return range(arr, num);
                }

                //! dot product of lhs[1..n] * rhs[1..n]
                template <typename LHS, typename RHS>
                inline T dot(LHS& lhs, RHS& rhs)
                {
                    assert(lhs.size() == rhs.size());
                    const size_t n = lhs.size();
                    resume(n);
                    for (size_t i = lhs.size(); i > 0; --i)
                    {
                        const_type l = lhs[i];
                        const_type r = rhs[i];
                        const_type p = l * r;
                        push_(p);
                    }
                    return sum();
                }

                //! dot product of lhs[0..n-1] * rhs[0..n-1]
                template <typename LHS, typename RHS>
                inline T dot(LHS *lhs, RHS *rhs, size_t n) {
                    assert( yack_good(lhs,n) );
                    assert( yack_good(rhs,n) );
                    resume(n);
                    while(n-- > 0) {
                        const_type l = *(lhs++);
                        const_type r = *(rhs++);
                        const_type p = l*r;
                        push_(p);
                    }
                    return sum();
                }


                //! a+b
                inline T operator()(const T a, const T b)
                {
                    return a+b;
                }

                //! a+b+c
                inline T operator()(const T a, const T b, const T c)
                {
                    resume(3); push_(a); push_(b); push_(c); return sum();
                }

                //! a+b+c+d
                inline T operator()(const T a, const T b, const T c, const T d)
                {
                    resume(4); push_(a); push_(b); push_(c); push_(d); return sum();
                }

                //! a+b+c+d+e
                inline T operator()(const T a, const T b, const T c, const T d, const T e)
                {
                    resume(5); push_(a); push_(b); push_(c); push_(d); push_(e); return sum();
                }

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(proto);
            };
        };

    }

}


#endif
