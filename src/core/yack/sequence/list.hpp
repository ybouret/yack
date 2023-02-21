//! \file

#ifndef YACK_LIST_INCLUDED
#define YACK_LIST_INCLUDED 1

#include "yack/container/as-capacity.hpp"
#include "yack/container/sequence.hpp"
#include "yack/container/writable.hpp"
#include "yack/container/iterator/linked.hpp"
#include "yack/data/dinky/solo-list.hpp"

namespace yack
{
    namespace kernel
    {
        extern const char list_category[]; //!< shared category name
    }
    
    //__________________________________________________________________________
    //
    //
    //! list of objects
    //
    //__________________________________________________________________________
    template <typename T>
    class list : public sequence<T>, public writable<T>
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type); //!< aliases
        typedef solo_list<T>                  impl_type; //!< implementation
        typedef typename impl_type::node_type node_type; //!< alias


        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! default setup empty
        inline explicit list() noexcept : sequence<T>(), writable<T>(), impl() {}

        //! cleanup is release
        inline virtual ~list() noexcept { release(); }

        //! setup empty, with capacity
        inline explicit list(const size_t n, const as_capacity_t &) noexcept :
        sequence<T>(), writable<T>(), impl()
        {
            impl.cache->reserve(n);
        }

        //! setup with size and default value
        inline explicit list(size_t n, param_type args) :
        sequence<T>(), writable<T>(), impl()
        {
            while(n-- > 0) impl << args;
        }


        //! hard copy of other list
        inline list(const list &other) : sequence<T>(), writable<T>(), impl(other.impl)
        {

        }


        //______________________________________________________________________
        //
        // collection interface
        //______________________________________________________________________
        inline virtual size_t      size()     const noexcept { return impl.size; }              //!< alive size
        inline virtual const char *category() const noexcept { return kernel::list_category; }  //!< category

        //______________________________________________________________________
        //
        // releasable interface
        //______________________________________________________________________
        inline virtual void     release()           noexcept { impl.clear(); impl.cache->release(); }  //!< release all nodes

        //______________________________________________________________________
        //
        // container interface
        //______________________________________________________________________
        inline virtual size_t   capacity()  const noexcept { return impl.size+impl.ready(); } //!< alive+cache
        inline virtual size_t   available() const noexcept { return impl.ready(); }           //!< cache only
        inline virtual void     free()            noexcept { impl.clear();           }        //!< remove content into cache
        inline virtual void     reserve(size_t n)         { impl.cache->reserve(n); }        //!< append zombie node into cache

        //______________________________________________________________________
        //
        // writable interface
        //______________________________________________________________________
        inline type       & operator[](const size_t indx) noexcept       { return **impl.get(indx); } //!< slow access
        inline const_type & operator[](const size_t indx) const noexcept { return **impl.get(indx); } //!< slow access

        //______________________________________________________________________
        //
        //! sequence interface
        //______________________________________________________________________
        inline virtual void push_back(param_type args)  { impl << args; }    //!< push back
        inline virtual void push_front(param_type args) { impl >> args; }    //!< push front
        inline virtual void pop_back()  noexcept         { impl.cut_tail(); } //!< pop back
        inline virtual void pop_front() noexcept         { impl.cut_head(); } //!< pop front


        //______________________________________________________________________
        //
        //! sort
        //______________________________________________________________________
        template <typename COMPARE_DATA> inline
        void sort(COMPARE_DATA &compare_data)
        {
            impl.sort_with(compare_data);
        }

        //______________________________________________________________________
        //
        //! reverse nodes
        //______________________________________________________________________
        inline void reverse() noexcept { impl.reverse(); }

        //______________________________________________________________________
        //
        // read only helpers
        //______________________________________________________________________

        inline const node_type *head() const noexcept { return impl.head; } //!< direct access
        inline const node_type *tail() const noexcept { return impl.tail; } //!< direct access

    private:
        YACK_DISABLE_ASSIGN(list);
        impl_type impl;


        inline virtual const_type *_front() const noexcept
        { assert(impl.head); return & (**impl.head); }
        
        inline virtual const_type *_back() const noexcept
        { assert(impl.tail); return & (**impl.tail); }

    public:
        typedef iterating::linked<type,node_type,iterating::forward> iterator;                           //!< forward iterator
        inline  iterator begin() noexcept { return impl.head; }                                           //!< forward iterator begin
        inline  iterator end()   noexcept { return 0;          }                                          //!< forward iterator end

        typedef iterating::linked<const_type,const node_type,iterating::forward> const_iterator;         //!< const forward iterator
        inline  const_iterator begin() const noexcept { return impl.head; }                               //!< const forward iterator begin
        inline  const_iterator end()   const noexcept { return 0;          }                              //!< const forward iterator end

        typedef iterating::linked<type,node_type,iterating::reverse> reverse_iterator;                   //!< reverse iterator
        inline  reverse_iterator rbegin() noexcept { return impl.tail; }                                  //!< reverse iterator begin
        inline  reverse_iterator rend()   noexcept { return 0;          }                                 //!< reverse iterator end

        typedef iterating::linked<const_type,const node_type,iterating::reverse> const_reverse_iterator; //!< const reverse iterator
        inline  const_reverse_iterator rbegin() const noexcept { return impl.tail; }                      //!< const reverse iterator begin
        inline  const_reverse_iterator rend()   const noexcept { return 0;          }                     //!< const reverse iterator end
    };
    
}


#endif

