//! \file

#ifndef YACK_HEAP_INCLUDED
#define YACK_HEAP_INCLUDED 1

#include "yack/ordered/priority-queue.hpp"
#include "yack/container.hpp"
#include "yack/container/as-copy.hpp"

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

        private:
            YACK_DISABLE_ASSIGN(heap);
        };


    }

    class compiled_buffer;
    class run_time_buffer;

    //__________________________________________________________________________
    //
    //
    //!
    //
    //__________________________________________________________________________
    template <typename T>
    class heap : public low_level::heap
    {
    public:
        YACK_DECL_ARGS(T,type);

        explicit heap() noexcept : low_level::heap()
        {
        }

        virtual ~heap() noexcept
        {
        }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(heap);

    };

}

#endif

