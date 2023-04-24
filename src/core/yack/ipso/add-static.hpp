
//! \file

#ifndef YACK_IPSO_ADD_STATIC_INCLUDED
#define YACK_IPSO_ADD_STATIC_INCLUDED 1

#include "yack/ipso/adding.hpp"
#include "yack/memory/allocator/dyadic.hpp"
#include "yack/ordered/compiled-buffer.hpp"

namespace yack
{
    namespace ipso
    {

        //______________________________________________________________________
        //
        //
        //! wrapper to compiled_buffer
        //
        //______________________________________________________________________
        template <typename T, const size_t N>
        struct compiled_memory
        {
            typedef compiled_buffer< typename inside<T>::type, N> type; //!< alias
        };

        //______________________________________________________________________
        //
        //
        //! adding with a fixed-size buffer
        //
        //______________________________________________________________________
        template <typename T, const size_t N>
        class static_add : public adding::proto<T, typename compiled_memory<T,N>::type >
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            YACK_DECL_ARGS(T,type);                                                     //!< aliases
            typedef typename inside<T>::type                               inside_type; //!< alias
            typedef adding::proto<T, typename compiled_memory<T,N>::type > proto_class; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            inline explicit static_add() noexcept : proto_class() {} //!< setup empty
            inline virtual ~static_add() noexcept {}                 //!< cleanup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(static_add);
        };


    }

}

#endif

