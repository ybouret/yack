//! \file

#ifndef YACK_DATA_SLIM_SOLO_REPO_INCLUDED
#define YACK_DATA_SLIM_SOLO_REPO_INCLUDED 1

#include "yack/data/slim/solo.hpp"
#include "yack/data/slim/manifest.hpp"
#include "yack/data/slim/ptr.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! list of addresses with internal cache
    //
    //__________________________________________________________________________
    template <typename T>
    class solo_repo : public slim_solo<T, slim_node< slim_ptr<T> >, slim_manifest >
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef slim_ptr<T>                          data_type; //!< alias
        typedef slim_node<data_type>                 node_type; //!< alias
        typedef slim_solo<T,node_type,slim_manifest> body_type; //!< alias

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit solo_repo() throw()  : body_type()  {} //!< setup
        inline virtual ~solo_repo() throw()                 {} //!< cleanup
        inline solo_repo(const solo_repo &_) : body_type(_) {} //!< cleanup

    private:
        YACK_DISABLE_ASSIGN(solo_repo);
    };

}

#endif
