//! \file

#ifndef YACK_COUNTING_SCHEDULE_INCLUDED
#define YACK_COUNTING_SCHEDULE_INCLUDED 1

#include "yack/counting/counting.hpp"
#include "yack/container/readable.hpp"
#include "yack/container/dynamic.hpp"

namespace yack
{
    
    class schedule : public counting,
    public readable<size_t>,
    public dynamic
    {
    public:
        virtual ~schedule() throw();
        
        virtual size_t granted() const throw();
        virtual size_t       & operator[](const size_t indx)       throw();
        virtual const size_t & operator[](const size_t indx) const throw();

        
    protected:
        explicit schedule(const size_t ctx_size,
                          const size_t max_size);
        void   *addr;
        size_t *data;
        
    private:
        size_t  used;
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(schedule);
    };
    
}

#endif

