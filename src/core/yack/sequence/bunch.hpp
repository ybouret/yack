//! \file
#ifndef YACK_BUNCH_INCLUDED
#define YACK_BUNCH_INCLUDED 1

#include "yack/sequence/cxx-array.hpp"
#include "yack/data/list/cxx.hpp"
#include "yack/object.hpp"

namespace yack
{

    template <typename T>
    class bunch
    {
    public:
        YACK_DECL_ARGS(T,type);
        typedef cxx_array<mutable_type> array_type;

        class entry : public object, public array_type
        {
        public:
            inline explicit entry(const size_t w) : array_type(w), next(0), prev(0) {}
            inline virtual ~entry() throw() {}

            entry *next;
            entry *prev;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(entry);
        };

        typedef cxx_list_of<entry> entries;


        inline explicit bunch(const size_t w) throw() :
        width(w)
        {
        }

        inline virtual ~bunch() throw() {}

        inline void reserve(size_t n)
        {
            while(n-- > 0) zpool.push_back( new entry(width) );
        }

        inline void free() throw()
        {
            zpool.merge_front(alive);
        }

        inline void release()  throw()
        {
            alive.release();
            zpool.release();
        }

        const entries * operator->() const throw() { return &alive; }
        const entries & operator*()  const throw() { return  alive; }

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

        template <typename ARRAY> inline
        void ensure(ARRAY &arr) {
            (void)insert(arr);
        }


        const size_t width;




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
    };

}

#endif

