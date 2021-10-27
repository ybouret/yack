//! \file

#ifndef YACK_DYNAMIC_INCLUDED
#define YACK_DYNAMIC_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    class dynamic
    {
    public:
        virtual ~dynamic() throw();

        virtual size_t granted() const throw() = 0; //!< linear space

    protected:
        explicit dynamic() throw();

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(dynamic);
    };

}

#endif

