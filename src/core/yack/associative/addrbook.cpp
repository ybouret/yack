
#include "yack/associative/addrbook.hpp"
#include "yack/associative/be-addr.hpp"

namespace yack
{
    addrbook:: ~addrbook() throw()
    {
    }
    
    addrbook::  addrbook() throw() : addrbook_()
    {
    }
    
    addrbook:: addrbook(const addrbook &other) : addrbook_(other)
    {
    }
    
    

    
    bool addrbook:: insert(const void *addr)
    {
        const be_address key(addr,as_address);
        return  addrbook_::insert((void*)addr,key.begin(),key.measure());
    }

    bool addrbook:: search(const void *addr) const throw()
    {
        const be_address key(addr,as_address);
        return NULL !=  addrbook_::search(key.begin(),key.measure());
    }

    void addrbook:: ensure(const void *addr)
    {
        const be_address key(addr,as_address);
        (void) addrbook_::insert((void*)addr,key.begin(),key.measure());
    }

    
}
