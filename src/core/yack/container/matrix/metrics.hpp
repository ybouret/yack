

#ifndef YACK_CONTAINER_MATRIX_METRICS_INCLUDED
#define YACK_CONTAINER_MATRIX_METRICS_INCLUDED 1

#include "yack/container/sequence/contiguous.hpp"

namespace yack
{
    
    template <typename T>
    class matrix_row : public contiguous<T>
    {
    public:
        typedef typename  writable<T>::type         type;
        typedef typename  writable<T>::const_type   const_type;
        typedef typename  writable<T>::mutable_type mutable_type;
       

        inline virtual size_t size() const throw() { return cols; }
        inline virtual type       &operator[](const size_t c) throw()       { assert(c>=1); assert(c<=cols); return item[c]; }
        inline virtual const_type &operator[](const size_t c) const throw() { assert(c>=1); assert(c<=cols); return item[c]; }
        
        inline virtual ~matrix_row() throw() {}
        inline explicit matrix_row(mutable_type *p,
                                   const size_t  c) throw() :
        cols(c),
        item(p)
        {
        }

        const size_t cols;

    private:
        mutable_type *item;
        YACK_DISABLE_COPY_AND_ASSIGN(matrix_row);
    };
    
    class matrix_metrics
    {
    public:
        typedef void (*row_callback)(void        *row_addr,
                                     void        *data_ptr,
                                     const size_t num_cols);
        
        virtual ~matrix_metrics() throw();
     
        
    protected:
        explicit matrix_metrics(void        **row_hook,
                                const size_t  num_rows,
                                const size_t  num_cols,
                                const size_t  size_of_item,
                                row_callback  build_row_at);
        
        explicit matrix_metrics() throw();
        void     deallocate() throw();

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(matrix_metrics);
        

    public:
        const size_t rows;
        const size_t cols;
        const size_t items;
        const size_t allocated;

    private:
        void         *workspace;
    };
    
}

#endif
