
//! \file

#ifndef YACK_ASSOCIATIVE_BE_ADDR_INCLUDED
#define YACK_ASSOCIATIVE_BE_ADDR_INCLUDED 1

#include "yack/memory/buffer/ro.hpp"

namespace yack
{

    class be_address : public memory::ro_buffer
    {
    public:
        virtual ~be_address() throw();

        template <typename T> inline
        explicit be_address(const T &args) throw() : impl(&args) { setup(); }

        virtual size_t      measure() const throw();
        virtual const void *ro_addr() const throw();

        friend std::ostream & operator<<(std::ostream &os, const be_address &);

    private:
        YACK_DISABLE_ASSIGN(be_address);
        const void *impl;

        void setup() throw();
    };

}


#endif
