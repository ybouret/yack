//! \file

#ifndef YACK_DATA_LINKED_INCLUDED
#define YACK_DATA_LINKED_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    
    //__________________________________________________________________________
    //
    //
    //! base class for linked classes
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

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(linked);
    };

}

#endif

