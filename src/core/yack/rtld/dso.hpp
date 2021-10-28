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
        explicit dso(const char *soname); //!< load library
        virtual ~dso() throw();           //!< free library

        //! load symbol address
        void    *load(const char *symbol) const throw();


        template <typename PROC>
        PROC query(const char *symbol) const throw()
        {
            union {
                void *addr;
                PROC  proc;
            } alias = { load(symbol) };
            return alias.proc;
        }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(dso);
        void  *handle;
    };

}

#endif

