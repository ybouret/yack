//! \file

#ifndef YACK_IPSO_ADD_INCLUDED
#define YACK_IPSO_ADD_INCLUDED 1

#include "yack/ipso/adding.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/ordered/run-time-buffer.hpp"

namespace yack
{
    namespace ipso
    {


        //______________________________________________________________________
        //
        //
        //! wrapper to run_time_buffer
        //
        //______________________________________________________________________
        template <typename T>
        struct run_time_memory
        {
            typedef run_time_buffer< typename inside<T>::type, memory::dyadic> type; //!< alias
        };

        //______________________________________________________________________
        //
        //
        //! adding with any memory
        //
        //______________________________________________________________________
        template <typename T>
        class add : public adding::proto<T, typename run_time_memory<T>::type >
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            YACK_DECL_ARGS(T,type);                                                   //!< alias
            typedef typename inside<T>::type                             inside_type; //!< alias
            typedef adding::proto<T, typename run_time_memory<T>::type > proto_class; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            inline explicit add() noexcept      : proto_class()  {} //!< setup empty
            inline explicit add(const size_t n) : proto_class(n) {} //!< setup with capacity
            inline virtual ~add() noexcept {}                       //!< cleanup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(add);
        };

    }
}

#endif

