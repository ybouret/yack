//! \file

#ifndef YACK_DATA_LINKED_INCLUDED
#define YACK_DATA_LINKED_INCLUDED 1

#include "yack/setup.hpp"
#include <iostream>

namespace yack
{
    
    //__________________________________________________________________________
    //
    //
    //! base class for classes of linked nodes
    //
    //__________________________________________________________________________
    class linked
    {
    public:
        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const size_t size;          //!< number of nodes
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~linked() noexcept;   //!< cleanup
        
    protected:
        explicit linked()   noexcept; //!< setup
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        void     increase() noexcept; //!< ++size
        void     decrease() noexcept; //!< --size
        void     xch_size(linked &other) noexcept; //!< exchange size values
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(linked);
    };
    
    //__________________________________________________________________________
    //
    //
    //! base class for interlinked nodes
    //
    //__________________________________________________________________________
    template <typename NODE>
    class interlinked  : public linked
    {
    public:
        //______________________________________________________________________
        //
        // virtual interface
        //______________________________________________________________________
        virtual bool owns(const NODE*) const noexcept = 0; //!< check ownership
        virtual void reverse()               noexcept = 0; //!< reverse order
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        
        //! check memory is increasing
        inline bool  memory_is_increasing() const noexcept
        {
            const NODE *node = head;
            for(size_t i=size;i>1;--i,node=node->next)
            {
                assert(NULL!=node);
                assert(NULL!=node->next);
                assert(node!=node->next);
                if(node->next<=node) return false;
            }
            return true;
        }
        
        
        //! check memory is decreasing
        inline bool  memory_is_decreasing() const noexcept
        {
            const NODE *node = head;
            for(size_t i=size;i>1;--i,node=node->next)
            {
                assert(NULL!=node);
                assert(NULL!=node->next);
                assert(node!=node->next);
                if(node<=node->next) return false;
            }
            return true;
        }

        //! generic display of node content
        inline friend std::ostream & operator<<(std::ostream &os, const interlinked &self)
        {
            const NODE *node = self.head;
            if(node)
            {
                assert(self.size>0);
                os << "{ " << *node;
                for(size_t i=self.size-1;i>0;--i)
                {
                    node=node->next;
                    os << ", " << *node;
                }
                os << " }";
            }
            else
                os << "{}";
            return os;
        }



        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        NODE *head; //!< head node for all interlinked
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~interlinked() noexcept {}
        
    protected:
        inline explicit interlinked() noexcept : linked(), head(0) {}
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(interlinked);
    };
    
}

#endif

