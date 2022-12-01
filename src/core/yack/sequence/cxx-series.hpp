
//! \file

#ifndef YACK_CXX_SERIES_INCLUDED
#define YACK_CXX_SERIES_INCLUDED 1

#include "yack/sequence/contiguous.hpp"
#include "yack/memory/allocator/pooled.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! linear memory without constructed objects, to be filled
    //
    //__________________________________________________________________________
    template <typename T, typename ALLOCATOR = memory::pooled>
    class cxx_series : public contiguous<T>
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type); //!< aliases


        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup empty linear memory
        inline explicit cxx_series(const size_t n) :
        bytes( 0 ),
        items( n ),
        entry( zget() ),
        count( 0 ),
        total( n ),
        basis( entry )
        {
            --entry;
        }

        //! clear empty linear memory
        inline virtual ~cxx_series() throw()
        {
            static memory::allocator &mem = ALLOCATOR::location();
            ++entry;
            while(count>0)
            {
                destruct( &entry[--coerce(count)] );
            }
            mem.withdraw(entry,coerce(bytes));
            coerce(items)=0;
            coerce(total)=0;
            basis        =0;
        }

        //______________________________________________________________________
        //
        // interface
        //______________________________________________________________________

        //! current count of objects
        inline virtual size_t size() const throw() { return count; }

        //! maximum count of objects
        inline size_t         capacity() const throw() { return total; }

        //! push back param_type
        inline void push_back( param_type args )
        {
            assert(count<total);
            new (basis+count) T(args);
            ++coerce(count);
        }

        //! helper
        inline cxx_series & operator<<(param_type args) {
            push_back(args);
            return *this;
        }

        //! one argument setup
        template <typename U> inline
        void add(typename type_traits<U>::parameter_type u)
        {
            new (basis+count) T(u);
            ++coerce(count);
        }

        //! two arguments setup
        template <typename U, typename V> inline
        void add(typename type_traits<U>::parameter_type u,
                 typename type_traits<V>::parameter_type v)
        {
            assert(count<total);
            new (basis+count) T(u,v);
            ++coerce(count);
        }

        //! three arguments setup
        template <typename U, typename V, typename W> inline
        void add(typename type_traits<U>::parameter_type u,
                 typename type_traits<V>::parameter_type v,
                 typename type_traits<W>::parameter_type w)
        {
            assert(count<total);
            new (basis+count) T(u,v,w);
            ++coerce(count);
        }

        //! pop/clean last item
        inline void pop() throw()
        {
            assert(count>0);
            out_of_reach::naught( destructed( &basis[--coerce(count)] ) );
        }

        //! pop all
        inline void free() throw() {
            while(count>0) pop();
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(cxx_series);
        const size_t      bytes; //!< allocated bytes
        const size_t      items; //!< allocated items>=count
        mutable_type     *entry; //!< shifted first item: entry[1..count]
        const size_t      count; //!< current count
        const size_t      total; //!< maximum count
        mutable_type     *basis; //!< first item

        inline mutable_type *zget()
        {
            static memory::allocator &mem = ALLOCATOR::instance();
            return mem.allocate<mutable_type>(coerce(items),coerce(bytes));
        }
        virtual const_type *cxx() const throw() { return entry; }
        virtual const_type *mem() const throw() { return basis; }

    };


}

#endif
