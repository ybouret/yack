
//! \file

#ifndef YACK_SYNC_LOOP_SIMD_INCLUDED
#define YACK_SYNC_LOOP_SIMD_INCLUDED 1

#include "yack/concurrent/loop.hpp"
#include "yack/concurrent/condition.hpp"
#include "yack/concurrent/mutex.hpp"

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
            virtual ~simd() throw();       //!< cleanup
            explicit simd(const size_t n); //!< setup

            //__________________________________________________________________
            //
            // readable interface
            //__________________________________________________________________
            virtual size_t       size()                   const throw();
            virtual const_type & operator[](const size_t) const throw();

            //__________________________________________________________________
            //
            // loop interface
            //__________________________________________________________________
            virtual lockable   & access()       throw();
            virtual const char * family() const throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(simd);
            class worker;
            
            mutex        sync;
            condition    cond;
            size_t       ready;
            const size_t threads;
            size_t       zbytes;  //!< bytes for workers
            worker      *team;    //!< memory for team
            condition    gate;

            void         cycle() throw();
            static void  entry(void *) throw();
            void         zkill() throw(); //!< return memory

        };

    }

}

#endif
