//! \file

#ifndef YACK_CAMEO_STATIC_ADD_INCLUDED
#define YACK_CAMEO_STATIC_ADD_INCLUDED 1

#include "yack/cameo/adding.hpp"
#include "yack/ordered/compiled-buffer.hpp"

namespace yack
{
    namespace cameo
    {
        //______________________________________________________________________
        //
        //
        //! adding with compiled time memory buffer
        //
        //______________________________________________________________________
        template <typename T, const size_t N>
        class static_add : public adding::proto<T, compiled_buffer<T,N> >
        {
        public:
            typedef adding::proto<T, compiled_buffer<T,N> > self_type; //!< alias

            inline explicit static_add() noexcept : self_type() {}     //!< setup
            inline virtual ~static_add() noexcept               {}     //!< cleanup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(static_add);
        };

    }

}

#endif
