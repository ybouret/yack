


//! \file

#ifndef YACK_CONTIGUOUS_INCLUDED
#define YACK_CONTIGUOUS_INCLUDED 1

#include "yack/container/writable.hpp"
#include "yack/sort/heap.hpp"
#include "yack/type/mswap.hpp"
#include "yack/container/iterator/linear.hpp"

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
        using writable<T>::size;


        //______________________________________________________________________
        //
        //! readable interface
        //______________________________________________________________________
        inline virtual const_type & operator[](const size_t indx) const throw()
        {
            assert(indx>=1); assert(indx<=size());
            assert(cxx());
            return *(cxx()+indx);
        }

        //______________________________________________________________________
        //
        //! writable interface
        //______________________________________________________________________
        inline virtual type & operator[](const size_t indx) throw()
        {
            assert(indx>=1); assert(indx<=size());
            assert(cxx());
            return coerce(*(cxx()+indx));
        }

        //______________________________________________________________________
        //
        // non virtual interface
        //______________________________________________________________________
        inline const_type * operator*() const throw() { return cxx();        } //!< direct [1..size()]
        inline type       * operator*()       throw() { return (type*)cxx(); } //!< direct [1..size()]


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

        //______________________________________________________________________
        //
        //! reverse order
        //______________________________________________________________________
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
        virtual const_type *cxx() const throw() = 0; //!< to access[1..size()]

    public:
#if 1
        //______________________________________________________________________
        //
        // iterators
        //______________________________________________________________________
        typedef iterating::linear<type,iterating::forward> iterator;
        inline  iterator begin() throw() { return **this+1; }
        inline  iterator end()   throw() { return (**this)+size()+1; }
        
        typedef iterating::linear<const_type,iterating::forward> const_iterator;
        inline  const_iterator begin() const throw() { return **this+1; }
        inline  const iterator end()   const throw() { return (**this)+size()+1; }
#endif
        
    };

}

#endif

