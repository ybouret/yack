
//! \file

#ifndef YACK_DINKY_COOP_LIST_INCLUDED
#define YACK_DINKY_COOP_LIST_INCLUDED 1

#include "yack/data/dinky/registry.hpp"
#include "yack/data/dinky/zcoop.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! list with shared cache
    //
    //__________________________________________________________________________
    template <typename T>
    class dinky_coop_list : public dinky_registry<T,zcoop>
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef dinky_registry<T,zcoop>        self_type;  //!< alias
        typedef typename self_type::zpool_type zpool_type; //!< alias
        typedef typename self_type::proxy_type proxy_type; //!< alias
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline  explicit dinky_coop_list(const proxy_type &_) throw() : self_type(_)  {} //!< setup
        inline  virtual ~dinky_coop_list() throw()                                    {} //!< cleanup
        inline  dinky_coop_list(const dinky_coop_list &_) : self_type(_)              {} //!< copy
        
        
    private:
        YACK_DISABLE_ASSIGN(dinky_coop_list);
    };
}

#endif

