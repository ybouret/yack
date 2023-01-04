

//! \file

#ifndef YACK_DATA_SLIM_COOP_LIST_INCLUDED
#define YACK_DATA_SLIM_COOP_LIST_INCLUDED 1

#include "yack/data/slim/coop.hpp"
#include "yack/data/slim/registry.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! list of objects with internal cache
    //
    //__________________________________________________________________________
    template <typename T>
    class coop_list : public slim_coop<T, slim_node<T>, yack::slim_registry>
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef slim_node<T>                               node_type; //!< alias
        typedef slim_coop<T,node_type,yack::slim_registry> body_type; //!< alias,
        typedef typename body_type::bank_type              bank_type; //!< alias
        typedef typename body_type::fund_type              fund_type; //!< alias
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit coop_list(const fund_type &fund) throw()  : body_type(fund)  {} //!< setup
        inline virtual ~coop_list() throw()                                          {} //!< cleanup
        inline coop_list(const coop_list &_) : body_type(_)                          {} //!< delegate copy

    private:
        YACK_DISABLE_ASSIGN(coop_list);
    };

}

#endif
