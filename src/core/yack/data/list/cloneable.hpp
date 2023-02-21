
//! \file

#ifndef YACK_DATA_CLONEABLE_LIST_INCLUDED
#define YACK_DATA_CLONEABLE_LIST_INCLUDED 1

#include "yack/data/list.hpp"
#include "yack/container/releasable.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! list of C++ cloneable nodes
    //
    //__________________________________________________________________________
    template <typename NODE>
    class cloneable_list_of : public list_of<NODE>, public releasable
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~cloneable_list_of() noexcept { release_(); }                      //!< cleanup by release
        inline explicit cloneable_list_of() noexcept : list_of<NODE>(), releasable() {}   //!< setup empty

        //! copy using NODE copy constructor
        inline          cloneable_list_of(const cloneable_list_of &other) : list_of<NODE>(), releasable()
        {
            try {
                for(const NODE *node=other.head;node;node=node->next)
                    this->push_back( node->clone() );
            }
            catch(...) { release_(); throw; }
        }

        //! assign using copy/swap
        inline cloneable_list_of & operator=(const cloneable_list_of &other)
        {
            cloneable_list_of tmp(other);
            swap_with(other);
            return *this;
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        //! delete all node
        inline virtual void release() noexcept
        {
            release_();
        }

        //! merge of copy of another list
        inline void merge_back_copy(const cloneable_list_of &other)
        {
            cloneable_list_of temp(other);
            this->merge_back(temp);
        }

        //! merge of copy of another list
        inline void merge_front_copy(const cloneable_list_of &other)
        {
            cloneable_list_of temp(other);
            this->merge_front(temp);
        }


    private:
        inline void release_() noexcept
        {
            while(this->size) delete this->pop_back();
        }
    };

}

#endif
