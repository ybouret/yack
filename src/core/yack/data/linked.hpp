//! \file

#ifndef YACK_DATA_LINKED_INCLUDED
#define YACK_DATA_LINKED_INCLUDED 1

#include "yack/setup.hpp"

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
        virtual ~linked() throw();   //!< cleanup
        
    protected:
        explicit linked()   throw(); //!< setup
        
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        void     increase() throw(); //!< ++size
        void     decrease() throw(); //!< --size
        void     xch_size(linked &other) throw(); //!< exchange size values
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
        inline virtual ~interlinked() throw() {}

        virtual bool owns(const NODE*) const throw() = 0; //!< check ownership
        virtual void reverse()               throw() = 0; //!< reverse order
        
    
        
        inline bool  memory_is_increasing() const throw()
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
        
        
        
        inline bool  memory_is_decreasing() const throw()
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
        
        NODE *head;
        
    protected:
        inline explicit interlinked() throw() : linked(), head(0) {}
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(interlinked);
    };

}

#endif

