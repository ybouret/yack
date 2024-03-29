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
    class solo_list : public dinky_registry<T,zsolo>, public releasable
    {
    public:
        //______________________________________________________________________
        //
        // types
        //______________________________________________________________________
        typedef dinky_registry<T,zsolo> self_type; //!< alias
        using self_type::cache;

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline  explicit solo_list() noexcept      : self_type()  {} //!< setup empty
        inline  explicit solo_list(const size_t n) : self_type()  { cache->reserve(n); } //!< setup with capacity
        inline  virtual ~solo_list() noexcept                     {} //!< cleanup
        inline  solo_list(const solo_list &_) : self_type(_)      {} //!< copy

        //! release all owned memory
        inline virtual void release() noexcept {
            this->clear();
            this->cache->release();
        }
        
    private:
        YACK_DISABLE_ASSIGN(solo_list);
    };
}

#endif

