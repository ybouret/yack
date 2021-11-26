
#include "yack/associative/be-addr.hpp"

namespace yack
{
    be_address:: ~be_address() throw()
    {
        impl = 0;
    }

    const void * be_address:: ro_addr() const throw()
    {
        return &impl;
    }

    size_t be_address:: measure() const throw()
    {
        return sizeof(void*);
    }

    std::ostream & operator<<(std::ostream &os, const be_address &a)
    {
        return a.display_hexa(os);
    }

    void be_address:: setup() throw()
    {
        
    }

}


