
#include "yack/associative/addrbook.hpp"
#include "yack/associative/be-addr.hpp"

namespace yack
{
    addrbook:: ~addrbook() throw()
    {
    }
    
    addrbook::  addrbook() throw() : db()
    {
    }
    
    addrbook:: addrbook(const addrbook &other) : db(other.db)
    {
    }
    
    
    const addrbook:: tree_type & addrbook:: operator*() const throw()
    {
        return db;
    }

    
    bool addrbook:: insert(const void *addr)
    {
        be_address key(addr,as_address);
        return db.insert((void*)addr,key.begin(),key.measure());
    }

    bool addrbook:: search(const void *addr) const throw()
    {
        be_address key(addr,as_address);
        return NULL != db.search(key.begin(),key.measure());
    }

    
}
