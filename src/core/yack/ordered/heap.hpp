//! \file

#ifndef YACK_HEAP_INCLUDED
#define YACK_HEAP_INCLUDED 1

#include "yack/ordered/prio-queue.hpp"
#include "yack/container.hpp"
#include "yack/container/as-copy.hpp"
#include "yack/type/args.hpp"
#include <iostream>

namespace yack
{

    namespace low_level
    {

        //______________________________________________________________________
        //
        //
        //! low-level heap for category and exceptions
        //
        //______________________________________________________________________
        class heap : public container
        {
        public:
            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual const char *category() const noexcept; //!< "heap"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~heap() noexcept; //!< cleanup

        protected:
            explicit heap() noexcept; //!< setup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void no_possible_reserve() const; //!< throw for non-versatile buffer

        private:
            YACK_DISABLE_ASSIGN(heap);
        };


    }



    //__________________________________________________________________________
    //
    //
    //! generic heap
    //
    //__________________________________________________________________________
    template <
    typename T,
    typename COMPARATOR,
    typename MEM_BUFFER>
    class heap : public low_level::heap
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);                             //!< aliases
        typedef low_level::heap               common_type;  //!< alias
        typedef prio_queue<T,COMPARATOR>      pqueue_type;  //!< alias
        typedef MEM_BUFFER                    buffer_type;  //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit heap() noexcept      : common_type(), M(),  Q(M.workspace,M.num_items) {} //!< default setup
        inline explicit heap(const size_t n) : common_type(), M(n), Q(M.workspace,M.num_items) {} //!< setup with required capacity
        inline virtual ~heap() noexcept {}

        //! generic copy with same comparator but maybe a different memory model
        template <typename OTH_BUFFER>
        inline heap(const heap<T,COMPARATOR,OTH_BUFFER> &other, const as_copy_t &) :
        M(other.size()), Q(M.workspace,M.num_items,other.getQ())
        {

        }

        //______________________________________________________________________
        //
        // container interface
        //______________________________________________________________________
        virtual inline void        free()            noexcept { Q.finish(); }
        virtual inline size_t      size()      const noexcept { return Q.count; }
        virtual inline size_t      capacity()  const noexcept { return Q.total; }
        virtual inline size_t      available() const noexcept { return Q.total - Q.count; }
        virtual inline void        reserve(size_t n)
        {
            static const int2type<MEM_BUFFER::versatile> behavior = {};
            reserve(behavior,n);
        }

        virtual inline void        release() noexcept
        {
            static const int2type<MEM_BUFFER::versatile> behavior = {};
            release(behavior);
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        //! access underlying priority queue, mostly for copy
        inline const pqueue_type & getQ() const noexcept { return Q; }

        //______________________________________________________________________
        //
        // heap API
        //______________________________________________________________________
        inline void push(param_type args)
        {
            Q.insert(args);
        }



    private:
        buffer_type M;
        pqueue_type Q;

        YACK_DISABLE_COPY_AND_ASSIGN(heap);

        // methods for compiled_buffer
        inline void release( const int2type<false> & ) noexcept { Q.finish(); }
        inline void reserve( const int2type<false> &, size_t)
        {
            no_possible_reserve();
        }


        // methods for run_time_buffer
        inline void release( const int2type<true>  & ) noexcept {
            Q.finish();
            if(Q.total>0)
            {
                buffer_type M0;
                pqueue_type Q0(M0.workspace,M0.num_items);
                M0.swap_with(M);
                Q0.swap_with(Q);
            }
        }

        inline void reserve(const int2type<true> &, size_t n)
        {
            if(n>0)
            {
                const size_t new_capacity = Q.total + n;
                buffer_type  m(new_capacity);
                pqueue_type  q(m.workspace,m.num_items); assert(q.total>Q.total);
                out_of_reach::swap(q.tree,Q.tree,(q.count=Q.count)*sizeof(type));
                Q.count = 0;
                m.swap_with(M);
                q.swap_with(Q);
            }
        }


    };


}

#endif

