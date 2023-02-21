
//! \file
#ifndef YACK_LINEAR_ITERATOR_INCLUDED
#define YACK_LINEAR_ITERATOR_INCLUDED 1

#include "yack/container/iterator.hpp"
#include "yack/type/args.hpp"

namespace yack
{

    namespace iterating
    {
        //______________________________________________________________________
        //
        //
        //! linear iterator
        //
        //______________________________________________________________________
        template <typename T,direction D>
        class linear
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            YACK_DECL_ARGS_(T,type); //!< aliases

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            inline ~linear()                noexcept { ptr=0; }                        //!< cleanup
            inline  linear(type *        _) noexcept : ptr( (mutable_type*)_)     {}   //!< setup
            inline  linear(const linear &_) noexcept : ptr( (mutable_type*)_.ptr) {}   //!< copy
            inline  linear&operator=(const linear &_) noexcept                         //|
            { ptr=(mutable_type *)_.ptr; return *this; }                              //!< assign

            //__________________________________________________________________
            //
            //! prefix increment operator
            //__________________________________________________________________
            inline linear & operator++() noexcept
            {
                static const int2type<D> way = {}; incr(way); return *this;
            }

            //__________________________________________________________________
            //
            //! postfix increment operator
            //__________________________________________________________________
            inline linear  operator++(int) noexcept
            {
                static const int2type<D> way = {};
                const linear             tmp = *this;
                incr(way);  return tmp;
            }

            //__________________________________________________________________
            //
            //! prefix decrement operator
            //__________________________________________________________________
            inline linear & operator--() noexcept
            {
                static const int2type<D> way = {}; decr(way); return *this;
            }

            //__________________________________________________________________
            //
            //! postfix decrement operator
            //__________________________________________________________________
            inline linear  operator--(int) noexcept
            {
                static const int2type<D> way = {};
                const linear tmp = *this; decr(way); return tmp;
            }

            //__________________________________________________________________
            //
            //! += n
            //__________________________________________________________________
            inline linear & operator+=(const ptrdiff_t n) noexcept
            {
                static const int2type<D> way = {};
                add(way,n); return *this;
            }

            //__________________________________________________________________
            //
            //! self + n
            //__________________________________________________________________
            inline friend bool operator+(const linear &lhs, const ptrdiff_t n) noexcept
            {
                linear tmp = lhs; tmp += n; return tmp;
            }

            //__________________________________________________________________
            //
            //! -= n
            //__________________________________________________________________
            inline linear & operator-=(const ptrdiff_t n) noexcept
            {
                static const int2type<D> way = {};
                sub(way,n); return *this;
            }

            //__________________________________________________________________
            //
            //! self - n
            //__________________________________________________________________
            inline friend bool operator-(const linear &lhs, const ptrdiff_t n) noexcept
            {
                linear tmp = lhs; tmp -= n; return tmp;
            }



            //__________________________________________________________________
            //
            //! equality
            //__________________________________________________________________
            inline friend bool operator==(const linear &lhs, const linear &rhs) noexcept
            {
                return lhs.ptr == rhs.ptr;
            }

            //__________________________________________________________________
            //
            //! difference
            //__________________________________________________________________
            inline friend bool operator!=(const linear &lhs, const linear &rhs) noexcept
            {
                return lhs.ptr != rhs.ptr;
            }

            //__________________________________________________________________
            //
            //! access
            //__________________________________________________________________
            inline type       & operator*()       noexcept { assert(ptr); return *ptr; }

            //__________________________________________________________________
            //
            //! access, const
            //__________________________________________________________________
            inline const_type & operator*() const noexcept { assert(ptr); return *ptr; }

        private:
            mutable_type *ptr;
            inline void incr(const int2type<forward> &) noexcept { ++ptr; }
            inline void incr(const int2type<reverse> &) noexcept { --ptr; }
            inline void decr(const int2type<forward> &) noexcept { --ptr; }
            inline void decr(const int2type<reverse> &) noexcept { ++ptr; }

            inline void add(const int2type<forward> &, const ptrdiff_t n) noexcept { ptr+=n; }
            inline void add(const int2type<reverse> &, const ptrdiff_t n) noexcept { ptr-=n; }
            inline void sub(const int2type<forward> &, const ptrdiff_t n) noexcept { ptr-=n; }
            inline void sub(const int2type<reverse> &, const ptrdiff_t n) noexcept { ptr+=n; }

        };




    }

}


#endif

