

//! \file

#ifndef YACK_LIST_INCLUDED
#define YACK_LIST_INCLUDED 1

#include "yack/container/as-capacity.hpp"
#include "yack/container/sequence.hpp"
#include "yack/container/writable.hpp"
#include "yack/container/iterator/linked.hpp"
#include "yack/data/slim/solo-list.hpp"

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
        inline explicit list() throw() : sequence<T>(), writable<T>(), impl() {}

        //! cleanup is release
        inline virtual ~list() throw() { release(); }

        //! setup empty, with capacity
        inline explicit list(const size_t n, const as_capacity_t &) throw() :
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
        inline virtual size_t      size()     const throw() { return impl.size; }              //!< alive size
        inline virtual const char *category() const throw() { return kernel::list_category; }   //!< categiry

        //______________________________________________________________________
        //
        // releasable interface
        //______________________________________________________________________
        inline virtual void     release()         throw()
        {
            impl.pluck();
            impl.cache->release();
        }  //!< release all nodes

        //______________________________________________________________________
        //
        // container interface
        //______________________________________________________________________
        inline virtual size_t   capacity()  const throw() { return impl.size+impl.cache->size; } //!< alive+cache
        inline virtual size_t   available() const throw() { return impl.cache->size; }           //!< cache only
        inline virtual void     free()            throw() { impl.free(); }                       //!< remove alive
        inline virtual void     reserve(size_t n)         { impl.cache->reserve(n); }            //!< append zombie node into cache

        //______________________________________________________________________
        //
        // writable interface
        //______________________________________________________________________
        inline type       & operator[](const size_t indx) throw()       { return **impl.get(indx); } //!< slow access
        inline const_type & operator[](const size_t indx) const throw() { return **impl.get(indx); } //!< slow access

        //______________________________________________________________________
        //
        //! sequence interface
        //______________________________________________________________________
        inline virtual void push_back(param_type args)  { impl << args; } //!< push back
        inline virtual void push_front(param_type args) { impl >> args; } //!< push front
        inline virtual void pop_back()  throw() { impl.zback();   }       //!< pop back
        inline virtual void pop_front() throw() { impl.zfront();  }       //!< pop front


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
        inline void reverse() throw() { impl.reverse(); }

        //______________________________________________________________________
        //
        // read only helpers
        //______________________________________________________________________

        inline const node_type *head() const throw() { return impl.head; } //!< direct access
        inline const node_type *tail() const throw() { return impl.tail; } //!< direct access

    private:
        YACK_DISABLE_ASSIGN(list);
        impl_type impl;


        inline virtual const_type *_front() const throw()
        { assert(impl.head); return & (**impl.head); }
        
        inline virtual const_type *_back() const throw()
        { assert(impl.tail); return & (**impl.tail); }

    public:
        typedef iterating::linked<type,node_type,iterating::forward> iterator;                           //!< forward iterator
        inline  iterator begin() throw() { return impl.head; }                                          //!< forward iterator begin
        inline  iterator end()   throw() { return 0;          }                                          //!< forward iterator end

        typedef iterating::linked<const_type,const node_type,iterating::forward> const_iterator;         //!< const forward iterator
        inline  const_iterator begin() const throw() { return impl.head; }                              //!< const forward iterator begin
        inline  const_iterator end()   const throw() { return 0;          }                              //!< const forward iterator end

        typedef iterating::linked<type,node_type,iterating::reverse> reverse_iterator;                   //!< reverse iterator
        inline  reverse_iterator rbegin() throw() { return impl.tail; }                                 //!< reverse iterator begin
        inline  reverse_iterator rend()   throw() { return 0;          }                                 //!< reverse iterator end

        typedef iterating::linked<const_type,const node_type,iterating::reverse> const_reverse_iterator; //!< const reverse iterator
        inline  const_reverse_iterator rbegin() const throw() { return impl.tail; }                     //!< const reverse iterator begin
        inline  const_reverse_iterator rend()   const throw() { return 0;          }                     //!< const reverse iterator end
    };
    
}


#endif

