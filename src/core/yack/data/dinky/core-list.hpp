
//! \file

#ifndef YACK_DINKY_CORE_LIST_INCLUDED
#define YACK_DINKY_CORE_LIST_INCLUDED 1

#include "yack/data/dinky/registry.hpp"
#include "yack/data/dinky/znone.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! list with no cache
    //
    //__________________________________________________________________________
    template <typename T>
    class  core_list : public dinky_registry<T,znone>
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef dinky_registry<T,znone> self_type; //!< alias
              
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline  explicit core_list() noexcept :  self_type()  {} //!< setup
        inline  virtual ~core_list() noexcept                 {} //!< cleanup
        inline  core_list(const core_list &_) : self_type(_) {} //!< copy
        
        
    private:
        YACK_DISABLE_ASSIGN(core_list);
    };
}

#endif

