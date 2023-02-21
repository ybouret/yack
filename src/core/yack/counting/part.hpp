//! \file

#ifndef YACK_COUNTING_PARTITION_INCLUDED
#define YACK_COUNTING_PARTITION_INCLUDED 1

#include "yack/counting/schedule.hpp"

namespace yack
{

    namespace apex
    {
        class natural;
    }

    //__________________________________________________________________________
    //
    //
    //! handling partitions of integers
    //
    //__________________________________________________________________________
    class partition : public schedule
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        explicit partition(const size_t n); //!< setup n>0
        virtual ~partition() noexcept;       //!< cleanup

        static apex::natural cardinality(const size_t n); //!< cardinality

        //______________________________________________________________________
        //
        // interface
        //______________________________________________________________________
        virtual size_t size() const noexcept; //!< current size in 1..n

        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(partition);
        virtual void on_boot() noexcept;       //!< size()=1, [1] = n
        virtual bool on_next() noexcept;       //!< up to [1 1...1 1]
    };
    
}

#endif
