//! \file

#ifndef YACK_DATA_SLIM_SOLO_INCLUDED
#define YACK_DATA_SLIM_SOLO_INCLUDED 1

#include "yack/data/slim/pool.hpp"

namespace yack
{

    template <typename T, typename NODE, template <typename,typename> class CATALOG>
    class slim_solo : CATALOG< T,slim_hook<NODE> >
    {
    public:
        explicit slim_solo() throw() {}
        virtual ~slim_solo() throw() {}

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(slim_solo);
    };

}

#endif

