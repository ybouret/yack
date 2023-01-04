
//! \file

#ifndef YACK_DATA_SLIM_COOP_INCLUDED
#define YACK_DATA_SLIM_COOP_INCLUDED 1

#include "yack/data/slim/bank.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! assembling classes for  catalog with shared pool
    //
    //__________________________________________________________________________
    template <typename T, typename NODE, template <typename,typename> class CATALOG>
    class slim_coop : public CATALOG<T, typename slim_bank<NODE>::pointer >
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef NODE                        node_type; //!< alias
        typedef slim_bank<node_type>        bank_type; //!< alias
        typedef typename bank_type::pointer fund_type; //!< alias
        typedef CATALOG<T,fund_type>        catalog_t; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit slim_coop(const fund_type &fund) throw() : catalog_t(fund) {} //!< setup with user's banck
        inline virtual ~slim_coop()                      throw()                   {} //!< cleanup
        inline          slim_coop(const slim_coop &other) : catalog_t(other)       {} //!< delegate copy

    private:
        YACK_DISABLE_ASSIGN(slim_coop);
    };

}

#endif

