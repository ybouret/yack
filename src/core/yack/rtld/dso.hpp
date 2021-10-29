#ifndef YACK_DSO_INCLUDED
#define YACK_DSO_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! system DLL helper
    //
    //__________________________________________________________________________
    class dso
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        explicit dso(const char *soname); //!< load library
        virtual ~dso() throw();           //!< free library

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! load symbol address
        void    *load(const char *symbol) const throw();

        //! helper to load proc
        template <typename CFUNCTION>
        CFUNCTION proc(const char *symbol) const throw()
        {
            union {
                void      *addr;
                CFUNCTION  proc;
            } alias = { load(symbol) };
            return alias.proc;
        }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(dso);
        void  *handle;
    };

}

#endif

