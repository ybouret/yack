//! \file

#ifndef YACK_DATA_SLIM_SOLO_INCLUDED
#define YACK_DATA_SLIM_SOLO_INCLUDED 1

#include "yack/data/slim/pool.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! assembling classes for solo (standalone) catalog
    //
    //__________________________________________________________________________
    template <typename T, typename NODE, template <typename,typename> class CATALOG>
    class slim_solo : public CATALOG< T,slim_hook<NODE> >
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef NODE                 node_type; //!< alias
        typedef slim_hook<node_type> hook_type; //!< alias
        typedef CATALOG<T,hook_type> catalog_t; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit slim_solo() throw() : catalog_t() {}            //!< setup empty
        inline virtual ~slim_solo() throw() {}                          //!< cleanup
        inline slim_solo(const slim_solo &other) : catalog_t(other) {}  //!< delegate copy

    private:
        YACK_DISABLE_ASSIGN(slim_solo);
    };

}

#endif

