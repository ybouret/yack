//! \file

#ifndef YACK_DATA_SLIM_SOLO_INCLUDED
#define YACK_DATA_SLIM_SOLO_INCLUDED 1

#include "yack/data/slim/pool.hpp"

namespace yack
{

    template <typename T, typename NODE, template <typename,typename> class CATALOG>
    class slim_solo : public CATALOG< T,slim_hook<NODE> >
    {
    public:
        typedef NODE                 node_type;
        typedef slim_hook<node_type> hook_type;
        typedef CATALOG<T,hook_type> catalog_t;

        inline explicit slim_solo() throw() : catalog_t() {}
        inline virtual ~slim_solo() throw() {}
        inline slim_solo(const slim_solo &other) : catalog_t(other) {}

    private:
        YACK_DISABLE_ASSIGN(slim_solo);
    };

}

#endif

