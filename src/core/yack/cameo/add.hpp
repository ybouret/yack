//! \file

#ifndef YACK_CAMEO_ADD_INCLUDED
#define YACK_CAMEO_ADD_INCLUDED 1

#include "yack/cameo/adding.hpp"
#include "yack/ordered/run-time-buffer.hpp"

namespace yack
{
    namespace cameo
    {

        //______________________________________________________________________
        //
        //
        //! adding with versatile memory buffer
        //
        //______________________________________________________________________
        template <typename T>
        class add : public adding::proto<T, run_time_buffer<T,allocator_type> >
        {
        public:
            typedef adding::proto<T, run_time_buffer<T,allocator_type> > self_type; //!< alias

            inline explicit add() noexcept      : self_type()  {} //!< setup empty
            inline explicit add(const size_t n) : self_type(n) {} //!< setup with capacity
            inline virtual ~add() noexcept                     {} //!< cleanup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(add);
        };

    }
}

#endif


