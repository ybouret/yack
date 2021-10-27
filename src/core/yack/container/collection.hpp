

#ifndef YACK_COLLECTION_INCLUDED
#define YACK_COLLECTION_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! collection interface: size and signed size
    //
    //__________________________________________________________________________
    class collection
    {
    public:
        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        virtual size_t      size()     const throw() = 0; //!< items
        unit_t              ssize()    const;              //!< signed size

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~collection() throw();
    protected:
        explicit collection() throw();
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(collection);
    };

}

#endif

