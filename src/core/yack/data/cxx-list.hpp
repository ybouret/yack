

//! \file

#ifndef YACK_DATA_CXX_LIST_INCLUDED
#define YACK_DATA_CXX_LIST_INCLUDED 1

#include "yack/data/list.hpp"
#include "yack/container/releasable.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! list of C++ objects
    //
    //__________________________________________________________________________
    template <typename NODE>
    class cxx_list_of : public list_of<NODE>
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        
      
        inline virtual ~cxx_list_of() throw() { release_(); }          //!< cleanup by release
        inline explicit cxx_list_of() throw() : list_of<NODE> () {}   //!< setup empty
     
        //! copy using NODE copy constructor
        inline          cxx_list_of(const cxx_list_of &other) : list_of<NODE>()
        {
            try {
                for(const NODE *node=other.head;node;node=node->next)
                    this->push_back( new NODE( *node ) );
            }
            catch(...) { release_(); throw; }
        }
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        //! delete all from tail
        inline virtual void release() throw()
        {
            release_();
        }
        
    private:
        YACK_DISABLE_ASSIGN(cxx_list_of);
        inline void release_() throw()
        {
            while(this->size) delete this->pop_back();
        }
    };
    
}

#endif
