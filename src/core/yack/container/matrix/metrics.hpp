

#ifndef YACK_CONTAINER_MATRIX_METRICS_INCLUDED
#define YACK_CONTAINER_MATRIX_METRICS_INCLUDED 1

#include "yack/container/matrix/row.hpp"
#include "yack/container/dynamic.hpp"
#include "yack/container/releasable.hpp"

namespace yack
{
    
    //__________________________________________________________________________
    //
    //
    //! metrics and raw memory allocation
    //
    //__________________________________________________________________________
    class matrix_metrics : public releasable, public dynamic
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef int2type<0> transposed_t; //!< helper for transposition

        //! how to create a raw
        typedef void (*row_callback)(void        *row_addr,
                                     void        *data_ptr,
                                     const size_t num_cols);
        

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        //! test metrics
        static bool have_same_sizes(const matrix_metrics &lhs, const matrix_metrics &rhs) noexcept;

        //! test tranposed metrics
        static bool are_transposed(const matrix_metrics &lhs, const matrix_metrics &rhs) noexcept;

        //! rows==cols
        bool is_square() const noexcept;

        //______________________________________________________________________
        //
        // dynamic interface
        //______________________________________________________________________
        virtual size_t granted() const noexcept; //!< allocated

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        virtual ~matrix_metrics() noexcept;

    protected:
        //! flexible setup
        explicit matrix_metrics(void        **row_hook,
                                void        **ptr_hook,
                                const size_t  num_rows,
                                const size_t  num_cols,
                                const size_t  size_of_item,
                                row_callback  build_row_at);

        //! setup empty
        explicit matrix_metrics() noexcept;

        //! no-throw swap
        void swap_metrics_with(matrix_metrics &other) noexcept;

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(matrix_metrics);
        void     deallocate() noexcept;


    public:
        const size_t rows;        //!< number of rows
        const size_t cols;        //!< number of cols
        const size_t items;       //!< number of items
        const size_t stride;      //!< cols * size_of_item
        
    private:
        size_t allocated; //!< allocated bytes
        void  *workspace; //!< allocated workspace
    };

    extern const matrix_metrics::transposed_t transposed; //!< make a transposed metrics

}

#endif
