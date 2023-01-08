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
    class dinky_solo_list : public dinky_registry<T,zsolo>
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
        inline  explicit dinky_solo_list() throw() :        self_type()  {} //!< setup
        inline  virtual ~dinky_solo_list() throw()                       {} //!< cleanup
        inline  dinky_solo_list(const dinky_solo_list &_) : self_type(_) {} //!< copy
        
        
    private:
        YACK_DISABLE_ASSIGN(dinky_solo_list);
    };
}

#endif

