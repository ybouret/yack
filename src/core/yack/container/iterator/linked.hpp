//! \file

#ifndef YACK_LINKED_ITERATOR_INCLUDED
#define YACK_LINKED_ITERATOR_INCLUDED 1

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
        template <typename T,typename NODE,direction D>
        class linked
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            YACK_DECL_ARGS_(T,type);         //!< aliases
            YACK_DECL_ARGS_(NODE,node_type); //!< aliases

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            inline ~linked()                  throw() { node=0; }                              //!< cleanup
            inline  linked(node_type *     n) throw() : node( (mutable_node_type*)n )       {} //!< setup
            inline  linked(const linked   &_) throw() : node( (mutable_node_type *) _.node) {} //!< copy
            inline  linked&operator=(const linked &_) throw()                                  //|
            {   node = (mutable_node_type*)_.node; return *this; }                             //!< assign

            //__________________________________________________________________
            //
            //! prefix increment operator
            //__________________________________________________________________
            inline linked & operator++() throw()
            {
                static const int2type<D> way = {}; incr(way); return *this;
            }

            //__________________________________________________________________
            //
            //! postfix increment operator
            //__________________________________________________________________
            inline linked  operator++(int) throw()
            {
                static const int2type<D> way = {};
                const linked             tmp = *this;
                incr(way);  return tmp;
            }

            //__________________________________________________________________
            //
            //! prefix decrement operator
            //__________________________________________________________________
            inline linked & operator--() throw()
            {
                static const int2type<D> way = {}; decr(way); return *this;
            }

            //__________________________________________________________________
            //
            //! postfix decrement operator
            //__________________________________________________________________
            inline linked  operator--(int) throw()
            {
                static const int2type<D> way = {};
                const linked tmp = *this; decr(way); return tmp;
            }

            //__________________________________________________________________
            //
            //! += n
            //__________________________________________________________________
            inline linked & operator+=(const ptrdiff_t n) throw()
            {
                static const int2type<D> way = {};
                add(way,n); return *this;
            }

            //__________________________________________________________________
            //
            //! self + n
            //__________________________________________________________________
            inline friend bool operator+(const linked &lhs, const ptrdiff_t n) throw()
            {
                linked tmp = lhs; tmp += n; return tmp;
            }

            //__________________________________________________________________
            //
            //! -= n
            //__________________________________________________________________
            inline linked & operator-=(const ptrdiff_t n) throw()
            {
                static const int2type<D> way = {};
                sub(way,n); return *this;
            }

            //__________________________________________________________________
            //
            //! self - n
            //__________________________________________________________________
            inline friend bool operator-(const linked &lhs, const ptrdiff_t n) throw()
            {
                linked tmp = lhs; tmp -= n; return tmp;
            }



            //__________________________________________________________________
            //
            //! equality
            //__________________________________________________________________
            inline friend bool operator==(const linked &lhs, const linked &rhs) throw()
            {
                return lhs.ptr == rhs.ptr;
            }

            //__________________________________________________________________
            //
            //! difference
            //__________________________________________________________________
            inline friend bool operator!=(const linked &lhs, const linked &rhs) throw()
            {
                return lhs.ptr != rhs.ptr;
            }

            //__________________________________________________________________
            //
            //! access
            //__________________________________________________________________
            inline type       & operator*()       throw() { assert(node); return **node; }

            //__________________________________________________________________
            //
            //! access, const
            //__________________________________________________________________
            inline const_type & operator*() const throw() { assert(node); return **node; }

        private:
            mutable_node_type *node;
            inline void incr(const int2type<forward> &) throw() { assert(node); node=node->next; }
            inline void incr(const int2type<reverse> &) throw() { assert(node); node=node->prev; }
            inline void decr(const int2type<forward> &) throw() { assert(node); node=node->prev; }
            inline void decr(const int2type<reverse> &) throw() { assert(node); node=node->next; }

            inline void add(const int2type<forward> &, ptrdiff_t n) throw()
            {
                if(n>0)
                {
                    while(n-- > 0 )
                    {
                        assert(node); node=node->next;
                    }
                }
                else
                {
                    if(n<0)
                    {
                        --n;
                        while(n-- > 0)
                        {
                            assert(node); node=node->prev;
                        }
                    }
                    else
                    {
                        assert(0==n);
                    }
                }
            }

            inline void add(const int2type<reverse> &, const ptrdiff_t n) throw()
            {
                static const int2type<forward> fwd = {};
                add(fwd,-n);
            }

            inline void sub(const int2type<forward> &, const ptrdiff_t n) throw()
            {
                static const int2type<forward> fwd = {};
                add(fwd,-n);
            }

            inline void sub(const int2type<reverse> &, const ptrdiff_t n) throw()
            {
                static const int2type<forward> fwd = {};
                add(fwd,n);
            }

        };




    }

}


#endif

