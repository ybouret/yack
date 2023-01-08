//! \file

#ifndef YACK_DINKY_SOLO_REPO_INCLUDED
#define YACK_DINKY_SOLO_REPO_INCLUDED 1

#include "yack/data/dinky/manifest.hpp"
#include "yack/data/dinky/zsolo.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! repo with own cache
    //
    //__________________________________________________________________________
    template <typename T>
    class solo_repo : public dinky_manifest<T,zsolo>
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef dinky_manifest<T,zsolo> self_type; //!< alias
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline  explicit solo_repo() throw()  : self_type()  {} //!< setup
        inline  virtual ~solo_repo() throw()                 {} //!< cleanup
        inline  solo_repo(const solo_repo &_) : self_type(_) {} //!< copy
        
        
    private:
        YACK_DISABLE_ASSIGN(solo_repo);
    };
}

#endif

