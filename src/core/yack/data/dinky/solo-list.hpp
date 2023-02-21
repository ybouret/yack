//! \file

#ifndef YACK_DINKY_SOLO_LIST_INCLUDED
#define YACK_DINKY_SOLO_LIST_INCLUDED 1

#include "yack/data/dinky/registry.hpp"
#include "yack/data/dinky/zsolo.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! list with own cache
    //
    //__________________________________________________________________________
    template <typename T>
    class solo_list : public dinky_registry<T,zsolo>
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef dinky_registry<T,zsolo> self_type; //!< alias
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline  explicit solo_list() noexcept  : self_type()  {} //!< setup
        inline  virtual ~solo_list() noexcept                 {} //!< cleanup
        inline  solo_list(const solo_list &_) : self_type(_) {} //!< copy
        
        
    private:
        YACK_DISABLE_ASSIGN(solo_list);
    };
}

#endif

