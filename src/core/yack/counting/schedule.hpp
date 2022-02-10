//! \file

#ifndef YACK_COUNTING_SCHEDULE_INCLUDED
#define YACK_COUNTING_SCHEDULE_INCLUDED 1

#include "yack/counting/counting.hpp"
#include "yack/container/readable.hpp"
#include "yack/container/dynamic.hpp"

namespace yack
{
    
    //__________________________________________________________________________
    //
    //
    //! counting with indices
    //
    //__________________________________________________________________________
    class schedule : public counting, public readable<size_t>, public dynamic
    {
    public:
        virtual ~schedule() throw(); //!< cleanup
        
        virtual size_t         granted()                const throw(); //!< dynamic interface
        virtual size_t       & operator[](const size_t)       throw(); //!< data[1..size()]
        virtual const size_t & operator[](const size_t) const throw(); //!< data[1..size()]
        
        
    protected:
        //! setup memory
        explicit schedule(const size_t        ctx_size,
                          const size_t        max_size,
                          const cardinality_t num);
        void   *addr; //!< context
        size_t *data; //!< data[1..max_size]
        
    private:
        size_t  used;
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(schedule);
    };
    
}

#endif

