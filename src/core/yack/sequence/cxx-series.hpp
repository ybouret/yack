
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
        inline virtual ~cxx_series() noexcept
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
        inline virtual size_t size() const noexcept { return count; }

        //! maximum count of objects
        inline size_t         capacity() const noexcept { return total; }

        //! push back param_type
        inline void push_back(param_type args)
        {
            assert(count<total);
            new (basis+count) T(args);
            ++coerce(count);
        }

        //! push front param_type
        inline void push_front(param_type args)
        {
            assert(count<total);
            lift(); try { new (basis) T(args); } catch(...) { fall(); throw; }
            ++coerce(count);
        }



        //! helper
        inline cxx_series & operator<<(param_type args) {
            push_back(args);
            return *this;
        }

        //! one argument setup, push_back
        template <typename U> inline
        type &add(typename type_traits<U>::parameter_type u)
        {
            assert(count<total);
            mutable_type *target = basis+count;
            new (target) T(u);
            ++coerce(count);
            return *target;
        }

        //! one argument setup, push_front
        template <typename U> inline
        type &pre(typename type_traits<U>::parameter_type u)
        {
            assert(count<total);
            lift(); try { new (basis) T(u); } catch(...) { fall(); throw; }
            ++coerce(count);
            return *basis;
        }


        //! one argument setup, insertion at given position
        template <typename U> inline
        type &ins(typename type_traits<U>::parameter_type u, const size_t pos)
        {
            assert(count<total);
            if(pos<=0)
            {
                return pre<U>(u);
            }
            else
            {
                if(pos>count)
                {
                    return add<U>(u);
                }
                else
                {
                    mutable_type *curr = &entry[pos];
                    mutable_type *next =  curr+1;
                    const size_t  nmov = (count+1-pos) * sizeof(type);
                    out_of_reach::move(next,curr,nmov);
                    try
                    {
                        new ( out_of_reach::naught(curr) ) T(u);
                        ++coerce(count);
                        return *curr;
                    }
                    catch(...)
                    {
                        out_of_reach::move(curr,next,nmov);
                        out_of_reach::naught(basis+count);
                        throw;
                    }
                }
            }

        }


        //! two arguments setup
        template <typename U, typename V> inline
        type & add(typename type_traits<U>::parameter_type u,
                   typename type_traits<V>::parameter_type v)
        {
            assert(count<total);
            mutable_type *target = basis+count;
            new (target) T(u,v);
            ++coerce(count);
            return *target;
        }

        //! three arguments setup
        template <typename U, typename V, typename W> inline
        type & add(typename type_traits<U>::parameter_type u,
                   typename type_traits<V>::parameter_type v,
                   typename type_traits<W>::parameter_type w)
        {
            assert(count<total);
            mutable_type *target = basis+count;
            new (target) T(u,v,w);
            ++coerce(count);
            return *target;
        }

        //! pop/clean last item
        inline void pop_back() noexcept
        {
            assert(count>0);
            out_of_reach::naught( destructed( &basis[--coerce(count)] ) );
        }

        //! pop all
        inline void free() noexcept {
            while(count>0) pop_back();
        }


        //! remove an item
        inline void suppress(const size_t indx) noexcept
        {
            assert(indx>=1);
            assert(indx<=size());
            mutable_type *target = entry+indx;
            destruct(target);
            out_of_reach::move(target,target+1,(count-indx)*sizeof(T));
            out_of_reach::naught( &basis[--coerce(count)] );
        }

        inline type       &front() noexcept       { assert(size()>0); return *basis; }       //!< first item
        inline const_type &front() const noexcept { assert(size()>0); return *basis; }       //!< first item, const
        inline type       &back()  noexcept       { assert(size()>0); return entry[count]; } //!< last item
        inline const_type &back()  const noexcept { assert(size()>0); return entry[count]; } //!< last item, const

        //! one argument setup, push_back
        template <typename U> inline
        void adjust(const size_t n, typename type_traits<U>::parameter_type u)
        {
            assert(n<=total);
            while(count>n) pop_back();
            while(count<n) {
                new (basis+count) T(u);
                ++coerce(count);
            }
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

        virtual const_type *cxx() const noexcept { return entry; }
        virtual const_type *mem() const noexcept { return basis; }

        inline void lift() noexcept
        {
            assert(count<total);
            const void  *source = basis;
            void        *target = basis+1;
            const size_t length = count * sizeof(type);
            out_of_reach::move(target,source,length);
        }

        inline void fall() noexcept
        {
            assert(count<total);
            const void  *source = basis+1;
            void        *target = basis;
            const size_t length = count * sizeof(type);
            out_of_reach::move(target,source,length);
        }


    };


}

#endif
