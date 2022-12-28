//! \file
#ifndef YACK_BUNCH_INCLUDED
#define YACK_BUNCH_INCLUDED 1

#include "yack/sequence/cxx-array.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/data/list/sort.hpp"
#include "yack/object.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! list of different integral coefficients
    //
    //__________________________________________________________________________
    template <typename T>
    class bunch
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);                     //!< aliases
        typedef cxx_array<mutable_type> array_type; //!< alias
        typedef readable<type>          readable_t; //!< default interface

        //______________________________________________________________________
        //
        //! one entry = a reusable array of coefficients
        //______________________________________________________________________
        class entry : public object, public array_type
        {
        public:
            
            inline explicit entry(const size_t w) : array_type(w), next(0), prev(0) {}//!< setup
            inline virtual ~entry() throw() {} //!< cleanup

            entry *next; //!< for list
            entry *prev; //!< for list

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(entry);
        };

        typedef cxx_list_of<entry> entries; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        
        //! setup empty
        inline explicit bunch(const size_t w) throw() :
        width(w), alive(), zpool(), work(w)
        {
        }

        //! cleanup
        inline virtual ~bunch() throw() {}

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        
        //! reserve entries
        inline void reserve(size_t n)
        {
            while(n-- > 0) zpool.push_back( new entry(width) );
        }

        //! free entries, keep memory
        inline void free() throw()
        {
            zpool.merge_front(alive);
        }

        //! release all
        inline void release()  throw()
        {
            alive.release();
            zpool.release();
        }
        
        const entries * operator->() const throw() { return &alive; } //!< access
        const entries & operator*()  const throw() { return  alive; } //!< access

        //! search array of same coefficients
        template <typename ARRAY> inline
        bool search(ARRAY &arr) const
        {
            assert(width==arr.size());
            for(const entry *ep=alive.head;ep;ep=ep->next)
            {
                if( are_same(arr,*ep) ) return true;
            }
            return false;
        }

        //! try to insert an array of coefficients
        template <typename ARRAY> inline
        bool insert(ARRAY &arr)
        {
            if(search(arr))
                return false;
            else
            {
                entry &tgt = * alive.push_back( query() );
                try {
                    for(size_t i=width;i>0;--i) tgt[i] = arr[i];
                }
                catch(...)
                {
                    zpool.push_back( alive.pop_back() );
                }
                return true;
            }
        }

        //! ensure an array of coefficients is present
        template <typename ARRAY> inline
        void ensure(ARRAY &arr) {
            (void)insert(arr);
        }

        //! sort using 'int proc(const readable_t &, const readable_t &)'
        template <typename COMPARE> inline
        void sort_with(COMPARE &proc)
        {
            callback<COMPARE> compare_entries = { proc };
            return merge_list_of<entry>::sort(alive,compare_entries);
        }


        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const size_t width; //!< number of coefficients per entry
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(bunch);
        entries alive; //!< in use
        entries zpool; //!< unsued

        inline entry  *query()
        {
            return zpool.size > 0 ? zpool.pop_back() : new entry(width);
        }

        template <typename ENTRY> inline
        bool are_same(ENTRY &lhs, const entry &rhs) const
        {
            assert(lhs.size()==width);
            assert(rhs.size()==width);
            for(size_t i=width;i>0;--i)
            {
                if(lhs[i]!=rhs[i]) return false;
            }
            return true;
        }

        template <typename COMPARE>
        struct callback
        {
            COMPARE &proc;
            inline int operator()(const entry *lhs, const entry *rhs)
            {
                assert(lhs); assert(rhs);
                return proc(*coerce_cast<readable_t,array_type>(lhs),*coerce_cast<readable_t,array_type>(rhs));
            }
        };


    public:
        array_type work; //!< workspace for possible conversions
    };

}

#endif

