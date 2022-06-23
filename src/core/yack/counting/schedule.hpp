//! \file

#ifndef YACK_COUNTING_SCHEDULE_INCLUDED
#define YACK_COUNTING_SCHEDULE_INCLUDED 1

#include "yack/counting/counting.hpp"
#include "yack/container/writable.hpp"
#include "yack/container/dynamic.hpp"
#include "yack/container/sequence.hpp"

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

        //! access proxy, const
        template <typename T> inline
        const T & operator()(const readable<T> &arr, const size_t i) const throw()
        {
            return arr[ (*this)[i] ];
        }

        //! access proxy
        template <typename T> inline
        T & operator()(writable<T> &arr, const size_t i) const throw()
        {
            return arr[ (*this)[i] ];
        }

        //! extract a subset
        template <typename T> inline
        void extract(sequence<T> &sub, const readable<T> &arr) const
        {
            const readable<size_t> &I = *this;
            const size_t            n = I.size();
            sub.free();
            for(size_t i=1;i<=n;++i)
                sub.push_back(arr[ I[i] ]);
        }


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

