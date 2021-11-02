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
    //! list of C++ nodes
    //
    //__________________________________________________________________________
    template <typename NODE>
    class cxx_list_of : public list_of<NODE>, public releasable
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~cxx_list_of() throw() { release_(); }                      //!< cleanup by release
        inline explicit cxx_list_of() throw() : list_of<NODE>(), releasable() {}   //!< setup empty
     
        //! copy using NODE copy constructor
        inline          cxx_list_of(const cxx_list_of &other) : list_of<NODE>(), releasable()
        {
            try {
                for(const NODE *node=other.head;node;node=node->next)
                    this->push_back( new NODE( *node ) );
            }
            catch(...) { release_(); throw; }
        }

        //! assign using copy/swap
        inline cxx_list_of & operator=(const cxx_list_of &other)
        {
            cxx_list_of tmp(other);
            swap_with(other);
            return *this;
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        //! delete all node
        inline virtual void release() throw()
        {
            release_();
        }

        //! merge of copy of another list
        inline void merge_back_copy(const cxx_list_of &other)
        {
            cxx_list_of temp(other);
            this->merge_back(other);
        }
        

    private:
        inline void release_() throw()
        {
            while(this->size) delete this->pop_back();
        }
    };
    
}

#endif
