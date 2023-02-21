//! \file

#ifndef YACK_CONTAINER_INCLUDED
#define YACK_CONTAINER_INCLUDED 1

#include "yack/container/collection.hpp"
#include "yack/container/releasable.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! container interface
    //
    //__________________________________________________________________________
    class container : public virtual collection, public releasable
    {
    public:
        //______________________________________________________________________
        //
        // virtual interface
        //______________________________________________________________________

        virtual            ~container()       noexcept;              //!< cleanup
        virtual const char *category()  const noexcept = 0;          //!< category
        virtual size_t      capacity()  const noexcept = 0;          //!< total item capacity
        virtual size_t      available() const noexcept = 0;          //!< available (memory for) items
        virtual void        free()            noexcept = 0;          //!< set size()=0, keep resources
        virtual void        reserve(size_t)           = 0;          //!< reserve room for extra items

        //______________________________________________________________________
        //
        // non virtual interface
        //______________________________________________________________________
        void ensure(const size_t minimal_capacity);  //!< ensure minimal capacity
        void resume(const size_t minimal_capacity);  //!< free/ensure

        //______________________________________________________________________
        //
        // helpers
        //______________________________________________________________________
        static size_t  next_capacity(const size_t capa);   //!< compute heuristic next capacity
        static size_t  next_increase(const size_t capa);   //!< compute heuristic next increase


    protected:
        explicit       container()        noexcept;         //!< setup
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(container);
        
    };
    
}


#endif

