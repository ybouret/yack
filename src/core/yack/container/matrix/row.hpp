
//! \file
//!
#ifndef YACK_CONTAINER_MATRIX_ROW_INCLUDED
#define YACK_CONTAINER_MATRIX_ROW_INCLUDED 1

#include "yack/sequence/contiguous.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! definition of a matrix row
    //
    //__________________________________________________________________________
    template <typename T>
    class matrix_row : public contiguous<T>
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef typename  writable<T>::type         type;           //!< alias
        typedef typename  writable<T>::const_type   const_type;     //!< alias
        typedef typename  writable<T>::mutable_type mutable_type;   //!< alias


        //______________________________________________________________________
        //
        // writable interface
        //______________________________________________________________________
        inline virtual size_t size() const throw() { return cols; }

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~matrix_row() throw() { coerce(cols)=0; item=0; }

        //! setup with offseted pointed
        inline explicit matrix_row(mutable_type *p, const size_t  c) throw() :
        cols(c),
        item(p)
        {
        }


        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        //! display as Julia matrix row
        inline void display(std::ostream &os) const {
            os << item[1]; for(size_t i=2;i<=cols;++i) os << ' ' << item[i];
        }

        //! return <this|rhs>
        template <typename U>
        inline type dot(const readable<U> &rhs) const
        {
            assert(cols==rhs.size());
            type sum = item[1]*rhs[1];
            for(size_t k=cols;k>1;--k) sum += item[k]*rhs[k];
            return sum;
        }

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const size_t cols; //!< associated number of colums


    private:
        mutable_type *item;
        YACK_DISABLE_COPY_AND_ASSIGN(matrix_row);
        inline virtual const_type *cxx() const throw() { return item;   }
        inline virtual const_type *mem() const throw() { return item+1; }

    };
}

#endif
