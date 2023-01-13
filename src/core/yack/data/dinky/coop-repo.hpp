
//! \file

#ifndef YACK_DINKY_COOP_REPO_INCLUDED
#define YACK_DINKY_COOP_REPO_INCLUDED 1

#include "yack/data/dinky/manifest.hpp"
#include "yack/data/dinky/zcoop.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! repo with own cache
    //
    //__________________________________________________________________________
    template <typename T>
    class  coop_repo : public dinky_manifest<T,zcoop>
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef dinky_manifest<T,zcoop>        self_type;  //!< alias
        typedef typename self_type::zpool_type zpool_type; //!< alias
        typedef typename self_type::proxy_type proxy_type; //!< alias

        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline  explicit coop_repo(const proxy_type &_) throw() : self_type(_)  {} //!< setup
        inline  virtual ~coop_repo() throw()                                    {} //!< cleanup
        inline  coop_repo(const coop_repo &_) : self_type(_)                    {} //!< copy
        
        
    private:
        YACK_DISABLE_ASSIGN(coop_repo);
    };
}

#endif

