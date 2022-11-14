//! \file

#ifndef YACK_COUNTING_BINARY_INCLUDED
#define YACK_COUNTING_BINARY_INCLUDED 1

#include "yack/counting/comb.hpp"
#include "yack/memory/workplace.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! handling binary representations
    //
    //__________________________________________________________________________
    class binary_spots : public counting, public readable<size_t >
    {
    public:
        virtual ~binary_spots() throw();
        explicit binary_spots(const size_t maxbits);

        virtual size_t         size()                 const throw();
        virtual const_type & operator[](const size_t) const throw();

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(binary_spots);
        combination                   *comb;
        const size_t                   bmax;
        memory::workplace<combination> wksp;

        void         restart() throw();
        virtual void on_boot() throw();
        virtual bool on_next();
        
    };


}

#endif

