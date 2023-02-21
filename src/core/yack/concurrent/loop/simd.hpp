
//! \file

#ifndef YACK_SYNC_LOOP_SIMD_INCLUDED
#define YACK_SYNC_LOOP_SIMD_INCLUDED 1

#include "yack/concurrent/loop.hpp"
#include "yack/concurrent/condition.hpp"
#include "yack/concurrent/mutex.hpp"
#include "yack/concurrent/topology.hpp"

namespace yack
{
    namespace concurrent
    {

        //______________________________________________________________________
        //
        //
        //! parallel loop
        //
        //______________________________________________________________________
        class simd : public loop
        {
        public:
            static const char clid[]; //!< simd

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~simd() noexcept;         //!< cleanup
            explicit simd(const topology &); //!< setup

            //__________________________________________________________________
            //
            // readable interface
            //__________________________________________________________________
            virtual size_t       size()                   const noexcept; //!< threads
            virtual const_type & operator[](const size_t) const noexcept; //!< context

            //__________________________________________________________________
            //
            // loop interface
            //__________________________________________________________________
            virtual lockable   & access()         noexcept;
            virtual const char * family()   const noexcept;
            virtual void operator()(kernel,void*) noexcept;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(simd);
            class worker;
            kernel       kcode;   //!< kernel code
            void        *kargs;   //!< kernel args
            size_t       live;    //!< working threads
            mutex        sync;    //!< synchro mutex
            condition    cond;    //!< synchro cond
            condition    gate;    //!< gate to synchronize with main thread
            const size_t threads; //!< number of threads
            size_t       zbytes_; //!< bytes for workers = context+thread
            worker      *squad;   //!< memory for squad of workers
            size_t       ready;   //!< use to create/delete threads
            
            void         cycle() noexcept;
            static void  entry(void *) noexcept;
            void         zkill() noexcept; //!< return memory
            void         finish(size_t count) noexcept;
        };

    }

}

#endif
