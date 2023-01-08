//! \file

#ifndef YACK_DINKY_CORE_REPO_INCLUDED
#define YACK_DINKY_CORE_REPO_INCLUDED 1

#include "yack/data/dinky/manifest.hpp"
#include "yack/data/dinky/znone.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! repo with no cache
    //
    //__________________________________________________________________________
    template <typename T>
    class core_repo : public dinky_manifest<T,znone>
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef dinky_manifest<T,znone> self_type; //!< alias
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline  explicit core_repo() throw()  : self_type()  {} //!< setup
        inline  virtual ~core_repo() throw()                 {} //!< cleanup
        inline  core_repo(const core_repo &_) : self_type(_) {} //!< copy
        
        
    private:
        YACK_DISABLE_ASSIGN(core_repo);
    };
}

#endif

