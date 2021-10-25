//! \file

#ifndef YACK_CONTAINER_MATRIX_INCLUDED
#define YACK_CONTAINER_MATRIX_INCLUDED 1

#include "yack/container/matrix/metrics.hpp"
#include "yack/type/destruct.hpp"
#include <iostream>

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! data for matrix
    //
    //__________________________________________________________________________
    template <typename T>
    class matrix_data
    {
    public:
        typedef matrix_row<T> row;                         //!< alias
        inline virtual ~matrix_data() throw() { row_=0; }  //!< cleanup

    protected:
        inline explicit matrix_data() throw() : row_(0) {} //!< setup
        row *row_;                                         //!< in [1..rows]

        void swap_with(matrix_data &other) throw()
        {
            cswap(row_,other.row_);
        }

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(matrix_data);
    };

    //__________________________________________________________________________
    //
    //
    //! data for matrix
    //
    //__________________________________________________________________________
    template <typename T>
    class matrix : public matrix_data<T>, public matrix_metrics
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS(T,type);                    //!< aliases
        typedef typename matrix_data<T>::row row;  //!< alias
        using matrix_data<T>::row_;
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~matrix() throw() { clear(items); }

        //! setup empty
        inline matrix() throw(): matrix_data<T>(), matrix_metrics() {}

        //! setup empty or not
        inline matrix(const size_t r, const size_t c) :
        matrix_data<T>(),
        matrix_metrics((void**)&row_,r,c,sizeof(T),build_row_at)
        {
            setup();
        }

        //! copy relying on copy constructor for type
        inline matrix(const matrix &M) :
        matrix_data<T>(),
        matrix_metrics((void**)&row_,M.rows,M.cols,sizeof(T),build_row_at)
        {
            setup_from(M);
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        //! row[1..rows]
        inline row &operator[](const size_t r) throw()
        { assert(r>=1); assert(r<=rows); return row_[r]; }

        //! row[1..rows] const
        inline const row &operator[](const size_t r) const throw()
        { assert(r>=1); assert(r<=rows); return row_[r]; }

        inline void xch(matrix &other) throw()
        {
            matrix_data<T>::swap_with(other);
            matrix_metrics::swap_with(other);
        }

    private:
        YACK_DISABLE_ASSIGN(matrix);

        //! creation of a row
        static inline void build_row_at(void *row_addr, void *data_ptr, const size_t num_cols) throw()
        { new (row_addr) row(static_cast<mutable_type *>(data_ptr),num_cols); }

        //! clear built objects
        inline void clear(size_t built) throw()
        {
            if(items>0)
            {
                mutable_type *data = & ((*this)[1][1]);
                while(built-- > 0)
                    destruct(data+built);
            }
        }

        //! setup items
        inline void setup()
        {
            if(items)
            {
                size_t built = 0;
                try {
                    mutable_type *data = & ((*this)[1][1]);
                    while(built<items)
                    {
                        new (data+built) mutable_type();
                        ++built;
                    }
                }
                catch(...)
                {
                    clear(built);
                    throw;
                }
            }
        }

        //! setup items
        inline void setup_from(const matrix &M)
        {
            assert( have_same_sizes(*this,M));
            if(items)
            {
                size_t built = 0;
                try {
                    const_type   *from = & (M[1][1]);
                    mutable_type *data = & ((*this)[1][1]);
                    while(built<items)
                    {
                        new (data+built) mutable_type(from[built]);
                        ++built;
                    }
                }
                catch(...)
                {
                    clear(built);
                    throw;
                }
            }
        }

    };
    
}

#endif

