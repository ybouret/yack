#ifndef YACK_DLL_INCLUDED
#define YACK_DLL_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! system DLL helper
    //
    //__________________________________________________________________________
    class dll
    {
    public:
        explicit dll(const char *soname); //!< load library
        virtual ~dll() throw();           //!< free library

        //! load symbol address
        void    *load(const char *symbol) const throw();


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(dll);
        void  *handle;
    };

}

#endif

