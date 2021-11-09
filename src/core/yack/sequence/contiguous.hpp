


//! \file

#ifndef YACK_CONTIGUOUS_INCLUDED
#define YACK_CONTIGUOUS_INCLUDED 1

#include "yack/container/writable.hpp"
#include "yack/sort/heap.hpp"
#include "yack/type/mswap.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! contiguous writable T
    //
    //__________________________________________________________________________
    template <typename T>
    class contiguous : public writable<T>
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef typename writable<T>::type       type;        //!< alias
        typedef typename writable<T>::const_type const_type;  //!< alias


        //______________________________________________________________________
        //
        // access methods
        //______________________________________________________________________
        virtual type       * operator*()       throw() = 0; //!< access to [1..size()]
        virtual const type * operator*() const throw() = 0; //!< access to [1..size()]

        //______________________________________________________________________
        //
        //! sorting
        //______________________________________________________________________
        template <typename COMPARE_DATA> inline
        void sort(COMPARE_DATA &compare_data)
        {
            writable<T> &self = *this;
            hsort(self,compare_data);
        }

        inline void reverse() throw()
        {
            writable<T> &self  = *this;
            size_t       upper = this->size();
            if(upper>=2)
            {
                size_t lower = 1;
                size_t count = upper>>1;
                while(count-- > 0)
                {
                    mswap(self[lower++],self[upper--]);
                }
            }
        }


        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~contiguous() throw() {} //!< cleanup

    protected:
        inline explicit contiguous() throw() {} //!< setup

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(contiguous);
    };

}

#endif

