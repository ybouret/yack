

//! \file

#ifndef YACK_DATA_CXX_POOL_INCLUDED
#define YACK_DATA_CXX_POOL_INCLUDED 1

#include "yack/data/pool.hpp"
#include "yack/container/releasable.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! pool of C++ objects
    //
    //__________________________________________________________________________
    template <typename NODE>
    class cxx_pool_of : public pool_of<NODE>, public releasable
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        
      
        inline virtual ~cxx_pool_of() throw() { release_(); }                      //!< cleanup by release
        inline explicit cxx_pool_of() throw() : pool_of<NODE>(), releasable() {}   //!< setup empty
     
        //! copy using NODE copy constructor
        inline          cxx_pool_of(const cxx_pool_of &other) : pool_of<NODE>(), releasable()
        {
            try {
                for(const NODE *node=other.head;node;node=node->next)
                    this->store( new NODE( *node ) );
                this->reverse();
            }
            catch(...) { release_(); throw; }
        }
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        inline virtual void release() throw()
        {
            release_();
        }
        
    private:
        YACK_DISABLE_ASSIGN(cxx_pool_of);
        inline void release_() throw()
        {
            while(this->size) delete this->query();
        }
    };
    
}

#endif
