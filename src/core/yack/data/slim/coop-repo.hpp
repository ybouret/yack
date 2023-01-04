//! \file

#ifndef YACK_DATA_SLIM_COOP_REPO_INCLUDED
#define YACK_DATA_SLIM_COOP_REPO_INCLUDED 1

#include "yack/data/slim/coop.hpp"
#include "yack/data/slim/manifest.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! list of addresses with shared cache
    //
    //__________________________________________________________________________
    template <typename T>
    class coop_repo : public slim_coop<T, slim_node< slim_ptr<T> >, yack::slim_manifest>
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef slim_ptr<T>                                data_type; //!< alias
        typedef slim_node<data_type>                       node_type; //!< alias
        typedef slim_coop<T,node_type,yack::slim_manifest> body_type; //!< alias,
        typedef typename body_type::bank_type              bank_type; //!< alias
        typedef typename body_type::fund_type              fund_type; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit coop_repo(const fund_type &fund) throw()  : body_type(fund)  {} //!< setup
        inline virtual ~coop_repo() throw()                                          {} //!< cleanup
        inline coop_repo(const coop_repo &_) : body_type(_)                          {} //!< delegate copy

    private:
        YACK_DISABLE_ASSIGN(coop_repo);
    };

}

#endif
