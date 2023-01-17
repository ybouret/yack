


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
        typedef typename writable<T>::type       type;           //!< alias
        typedef typename writable<T>::const_type const_type;     //!< alias
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
        
        inline const_type *operator()(void)              const throw() { return mem();            } //!< first const object
        inline type       *operator()(void)                    throw() { return (type*)mem();     } //!< first object
        inline const_type *operator()(const ptrdiff_t n) const throw() { return mem()+n;          } //!< shifted first const object
        inline type       *operator()(const ptrdiff_t n)       throw() { return (type*)(mem()+n); } //!< shifted first object

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
            size_t       upper = size();
            if(upper>=2)
            {
                size_t lower = 1;
                size_t count = upper>>1;
                while(count-- > 0)
                    mswap(self[lower++],self[upper--]);
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
        virtual const_type *mem() const throw() = 0; //!< to access[0..size()-1]

    public:
        //______________________________________________________________________
        //
        // iterators
        //______________________________________________________________________
        typedef iterating::linear<type,iterating::forward> iterator;                     //!< forward iterator
        inline  iterator begin() throw() { return (type*)mem();          }               //!< forward iterator begin
        inline  iterator end()   throw() { return ((type*)mem())+size(); }               //!< forward iterator end

        typedef iterating::linear<const_type,iterating::forward> const_iterator;         //!< const forward iterator
        inline  const_iterator begin() const throw() { return mem(); }                   //!< const forward iterator begin
        inline  const_iterator end()   const throw() { return mem()+size(); }            //!< const forward iterator end

        typedef iterating::linear<type,iterating::reverse> reverse_iterator;             //!< reverse iterator
        inline  reverse_iterator rbegin() throw() { return ((type *)cxx())+size();  }    //!< reverse iterator begin
        inline  reverse_iterator rend()   throw() { return (type*)cxx();            }    //!< reverse iterator end

        typedef iterating::linear<const_type,iterating::reverse> const_reverse_iterator; //!< const reverse iterator
        inline  const_reverse_iterator rbegin() const throw() { return cxx()+size();  }  //!< const reverse iterator begin
        inline  const_reverse_iterator rend()   const throw() { return cxx();         }  //!< const reverse iterator end


    };

}

#endif

