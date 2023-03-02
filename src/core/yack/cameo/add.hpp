//! \file

#ifndef YACK_CAMEO_ADD_INCLUDED
#define YACK_CAMEO_ADD_INCLUDED 1

#include "yack/cameo/adding.hpp"
#include "yack/ordered/run-time-buffer.hpp"

namespace yack
{
    namespace cameo
    {

      
        template <typename T>
        class add : public adding::proto<T, run_time_buffer<T,allocator_type> >
        {
        public:
            typedef adding::proto<T, run_time_buffer<T,allocator_type> > self_type;

            inline explicit add() noexcept      : self_type()  {}
            inline explicit add(const size_t n) : self_type(n) {}
            inline virtual ~add() noexcept                     {}

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(add);
        };

    }
}

#endif


