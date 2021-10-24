//! \file

#ifndef YACK_CONTAINER_MATRIX_INCLUDED
#define YACK_CONTAINER_MATRIX_INCLUDED 1

#include "yack/container/matrix/metrics.hpp"

namespace yack
{
    
    template <typename T>
    class matrix : public matrix_metrics
    {
    public:
        YACK_DECL_ARGS(T,type);
        typedef matrix_row<T> row;
        
        
        inline virtual ~matrix() throw() {}
        inline explicit matrix() throw(): matrix_metrics(sizeof(T)) {}
        inline explicit matrix(const size_t r, const size_t c) :
        matrix_metrics((void**)&row_,r,c,sizeof(T))
        {
            for(size_t r=1;r<=rows;++r)
            {
                assert(row_[r].cols==cols);
            }
        }
        
        inline row &operator[](const size_t r) throw()
        { assert(r>=1); assert(r<=rows); return row_[r]; }
        
        inline const row &operator[](const size_t r) const throw()
        { assert(r>=1); assert(r<=rows); return row_[r]; }
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(matrix);
        row *row_;
        
        
    };
    
}

#endif

