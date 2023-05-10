
//! \file

#ifndef YACK_DATA_CXX_CLIST_INCLUDED
#define YACK_DATA_CXX_CLIST_INCLUDED 1

#include "yack/data/clist.hpp"
#include "yack/container/releasable.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! circular list of C++ object
    //
    //__________________________________________________________________________
    template <typename NODE>
    class cxx_clist_of : public clist_of<NODE>, public releasable
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //_______________________________________________________________________

        //! setup empty
        inline explicit cxx_clist_of() noexcept : clist_of<NODE>(), releasable() {}

        //! cleanup
        inline virtual ~cxx_clist_of() noexcept { release_(); }

        //! copy
        inline cxx_clist_of(const cxx_clist_of &other) : clist_of<NODE>(), releasable()
        {
            try {
                const NODE *node = other.head;
                for(size_t i=other.size;i>0;--i,node-node->next)
                {
                    this->push_back( new NODE( *node ) );
                }
            }
            catch(...)
            {
                release_(); throw;
            }
        }


        //______________________________________________________________________
        //
        // methods
        //_______________________________________________________________________

        //! release all objects
        inline virtual void release() noexcept { release_(); }

    private:
        YACK_DISABLE_ASSIGN(cxx_clist_of);
        inline void release_() noexcept {
            while( this->size ) delete this->pop_back();
        }
    };

}

#endif

