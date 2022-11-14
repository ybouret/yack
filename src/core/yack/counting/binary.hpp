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
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        explicit binary_spots(const size_t maxbits); //!< setup
        virtual ~binary_spots() throw();             //!< cleanup

        size_t                capacity()              const throw(); //!< maxbits
        virtual size_t        size()                  const throw(); //!< in [0:maxbits]
        virtual const_type & operator[](const size_t) const throw(); //!< [] to [1,2,...,maxbits]

        template <typename T> inline
        void fill(writable<T>                     &arr,
                  typename writable<T>::const_type zero=0,
                  typename writable<T>::const_type ones=1)
        {
            arr.ld(zero);
            const readable<size_t> &self = *this;
            for(size_t i=self.size();i>0;--i)
            {
                arr[ self[i] ] = ones;
            }
        }

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

