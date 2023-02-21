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
        //______________________________________________________________________
        //
        // interface
        //______________________________________________________________________
        virtual size_t         granted()                const noexcept; //!< dynamic interface
        virtual size_t       & operator[](const size_t)       noexcept; //!< data[1..size()]
        virtual const size_t & operator[](const size_t) const noexcept; //!< data[1..size()]

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! access proxy, const
        template <typename T> inline
        const T & operator()(const readable<T> &arr, const size_t i) const noexcept
        {
            return arr[ (*this)[i] ];
        }

        //! access proxy
        template <typename T> inline
        T & operator()(writable<T> &arr, const size_t i) const noexcept
        {
            return arr[ (*this)[i] ];
        }

        //! extract a subset into a sequence by constrution
        template <typename T> inline
        void construct(sequence<T> &sub, const readable<T> &arr) const
        {
            const readable<size_t> &I = *this;
            const size_t            n = I.size();
            sub.free();
            for(size_t i=1;i<=n;++i)
                sub.push_back(arr[ I[i] ]);
        }

        //! extract a subset into a sequence by copy
        template <typename TARGET, typename SOURCE> inline
        void designate(TARGET &target, SOURCE &source)
        {
            const readable<size_t> &self = *this;
            for(size_t i=self.size();i>0;--i) {
                target[i] = source[ self[i] ];
            }
        }
        
        //! lineary search of value within current indices
        template <typename T> inline
        bool look_up(T &value) const {
            const readable<size_t> &self = *this;
            for(size_t i=self.size();i>0;--i) {
                if( self[i] == value ) return true;
            }
            return false;
        }

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        virtual ~schedule() noexcept;

    protected:
        //! setup memory
        explicit schedule(const size_t        ctx_size,
                          const size_t        max_size,
                          const cardinality_t num);

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        void   *addr; //!< context
        size_t *data; //!< data[1..max_size]
        
    private:
        size_t  used;
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(schedule);
    };
    
}

#endif

