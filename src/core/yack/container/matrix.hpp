//! \file

#ifndef YACK_CONTAINER_MATRIX_INCLUDED
#define YACK_CONTAINER_MATRIX_INCLUDED 1

#include "yack/container/matrix/metrics.hpp"
#include <iostream>

namespace yack
{
    template <typename T>
    class matrix_
    {
    public:
        typedef matrix_row<T> row;

        inline virtual ~matrix_() throw() {}

    protected:
        inline explicit matrix_() throw() : row_(0) {}
        row *row_;

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(matrix_);
    };

    template <typename T>
    class matrix : public matrix_<T>, public matrix_metrics
    {
    public:
        YACK_DECL_ARGS(T,type);
        typedef typename matrix_<T>::row row;
        using matrix_<T>::row_;
        
        
        inline virtual ~matrix() throw() {}
        inline explicit matrix() throw(): matrix_metrics() {}
        inline explicit matrix(const size_t r, const size_t c) :
        matrix_metrics((void**)&row_,r,c,sizeof(T),build_row_at)
        {
        }

        inline row &operator[](const size_t r) throw()
        { assert(r>=1); assert(r<=rows); return row_[r]; }
        
        inline const row &operator[](const size_t r) const throw()
        { assert(r>=1); assert(r<=rows); return row_[r]; }
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(matrix);
        static inline void build_row_at(void *row_addr, void *data_ptr, const size_t num_cols) throw()
        { new (row_addr) row(static_cast<mutable_type *>(data_ptr),num_cols); }
        
    };
    
}

#endif

