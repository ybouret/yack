
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
    class dinky_coop_repo : public dinky_manifest<T,zcoop>
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
        inline  explicit dinky_coop_repo(const proxy_type &_) throw() : self_type(_)  {} //!< setup
        inline  virtual ~dinky_coop_repo() throw()                                    {} //!< cleanup
        inline  dinky_coop_repo(const dinky_coop_repo &_) : self_type(_)              {} //!< copy
        
        
    private:
        YACK_DISABLE_ASSIGN(dinky_coop_repo);
    };
}

#endif

