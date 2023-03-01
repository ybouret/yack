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
            void push_on_memory_full() const; //!< throw for full non-versatile buffer

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
        inline explicit heap() noexcept      : common_type(), my(),  pq(my.workspace,my.num_items) {} //!< default setup
        inline explicit heap(const size_t n) : common_type(), my(n), pq(my.workspace,my.num_items) {} //!< setup with required capacity
        inline virtual ~heap() noexcept {}

        //! generic copy with same comparator but maybe a different memory model
        template <typename OTH_BUFFER>
        inline heap(const heap<T,COMPARATOR,OTH_BUFFER> &other, const as_copy_t &) :
        my(other.size()), pq(my.workspace,my.num_items,other.getQ())
        {

        }

        //! generic copy with same comparator but maybe a different memory model and extra
        template <typename OTH_BUFFER>
        inline heap(const heap<T,COMPARATOR,OTH_BUFFER> &other, const as_copy_t &, const size_t extra) :
        my(other.size()+extra), pq(my.workspace,my.num_items,other.getQ())
        {

        }

        //______________________________________________________________________
        //
        // container interface
        //______________________________________________________________________
        virtual inline void        free()            noexcept { pq.finish(); }
        virtual inline size_t      size()      const noexcept { return pq.count; }
        virtual inline size_t      capacity()  const noexcept { return pq.total; }
        virtual inline size_t      available() const noexcept { return pq.total - pq.count; }
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
        inline const pqueue_type & getQ() const noexcept { return pq; }

        //! display using local heap copy
        inline friend std::ostream & operator<<( std::ostream &os, const heap &self)
        {
            if(self.pq.count<=0)
            {
                os << "{}";
            }
            else
            {
                heap h(self,as_copy); assert(h.size()>=1);
                os << "{";
                os << h.pull();
                while(h.size()) os << ", " << h.pull();
                os << " }";
            }
            return os;
        }

        //______________________________________________________________________
        //
        // heap API
        //______________________________________________________________________

        //! push with enough memory
        inline void push_(param_type args)
        {
            assert(pq.count<pq.total);
            pq.insert(args);
        }

        //! push with safe memory
        inline void push(param_type args)
        {
            static const int2type<MEM_BUFFER::versatile> behavior = {};
            push(behavior,args);
        }

        //! pop
        inline void pop() noexcept
        {
            assert(pq.count>0);
            assert(pq.tree!=0);
            pq.remove();
        }

        //! peek highest priority value
        inline const_type & peek() const noexcept
        {
            assert(pq.count>0);
            assert(pq.tree!=0);
            return pq.tree[0];
        }

        //! pull highest priority value
        inline mutable_type pull() {
            assert(pq.count>0);
            assert(pq.tree!=0);
            const_type res = pq.tree[0];
            pq.remove();
            return res;
        }



    private:
        buffer_type my;
        pqueue_type pq;

        YACK_DISABLE_COPY_AND_ASSIGN(heap);

        //______________________________________________________________________
        //
        // methods for compiled_buffer
        //______________________________________________________________________
        inline void release( const int2type<false> & ) noexcept { pq.finish(); }
        inline void reserve( const int2type<false> &, size_t) { no_possible_reserve(); }
        inline void push( const int2type<false> &, const_type &args )
        {
            if(pq.count>=pq.total) push_on_memory_full();
            pq.insert(args);
        }

        //______________________________________________________________________
        //
        // methods for run_time_buffer
        //______________________________________________________________________
        inline void release( const int2type<true>  & ) noexcept {
            pq.finish();
            if(pq.total>0)
            {
                buffer_type M0;
                pqueue_type Q0(M0.workspace,M0.num_items);
                M0.swap_with(my);
                Q0.swap_with(pq);
            }
        }

        inline void reserve(const int2type<true> &, size_t n)
        {
            if(n>0)
            {
                // create local heap with more memory
                heap h(pq.total+n);

                // give it full control
                out_of_reach::swap(pq.tree,h.pq.tree,(h.pq.count=pq.count)*sizeof(type));
                pq.count = 0;

                // exchange
                pq.swap_with(h.pq);
                my.swap_with(h.my);

            }
        }


        inline void push( const int2type<true> &, const_type &args )
        {
            const size_t capa = pq.total;
            if(pq.count<capa)
                pq.insert(args);
            else
            {
                // larger hard copy
                heap h(*this,as_copy,next_increase(capa));

                // insert in new heap
                h.pq.insert(args);

                // exchange
                pq.swap_with(h.pq);
                my.swap_with(h.my);
            }
        }

    };


}

#endif

