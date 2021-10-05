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

    protected:
        inline explicit interlinked() throw() : linked() {}

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(interlinked);
    };

}

#endif

