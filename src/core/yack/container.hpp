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
        virtual            ~container()       throw();     //!< cleanup
        virtual const char *category()  const throw() = 0; //!< category
        virtual size_t      capacity()  const throw() = 0; //!< total item capacity
        virtual size_t      available() const throw() = 0; //!< available (memory for) items
        virtual void        free()            throw() = 0; //!< set size()=0, keep resources
        virtual void        reserve(size_t)           = 0; //!< reserve room for extra items

        static size_t  next_capacity(const size_t capa);   //!< compute heuristic next capacity
        static size_t  next_increase(const size_t capa);   //!< compute heuristic next increase

    protected:
        explicit       container()        throw();         //!< setup
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(container);
        
    };
    
}


#endif

