

#ifndef YACK_CONTAINER_MATRIX_METRICS_INCLUDED
#define YACK_CONTAINER_MATRIX_METRICS_INCLUDED 1

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



        inline virtual size_t size() const throw() { return cols; }
        inline virtual type       &operator[](const size_t c) throw()       { assert(c>=1); assert(c<=cols); return item[c]; }
        inline virtual const_type &operator[](const size_t c) const throw() { assert(c>=1); assert(c<=cols); return item[c]; }

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        inline virtual ~matrix_row() throw() {}

        //! setup with offseted pointed
        inline explicit matrix_row(mutable_type *p, const size_t  c) throw() :
        cols(c),
        item(p)
        {
        }

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const size_t cols; //!< associated number of colums

    private:
        mutable_type *item;
        YACK_DISABLE_COPY_AND_ASSIGN(matrix_row);
    };

    //__________________________________________________________________________
    //
    //
    //! metrics and raw memory allocation
    //
    //__________________________________________________________________________
    class matrix_metrics
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________

        //! how to create a raw
        typedef void (*row_callback)(void        *row_addr,
                                     void        *data_ptr,
                                     const size_t num_cols);
        

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        static bool have_same_sizes(const matrix_metrics &lhs, const matrix_metrics &rhs) throw();

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! cleanup
        virtual ~matrix_metrics() throw();

    protected:
        //! flexible setup
        explicit matrix_metrics(void        **row_hook,
                                const size_t  num_rows,
                                const size_t  num_cols,
                                const size_t  size_of_item,
                                row_callback  build_row_at);

        //! setup empty
        explicit matrix_metrics() throw();

        //! no-throw swap
        void swap_with(matrix_metrics &other) throw();

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(matrix_metrics);
        void     deallocate() throw();


    public:
        const size_t rows;        //!< number of rows
        const size_t cols;        //!< number of cols
        const size_t items;       //!< number of items
        const size_t allocated;   //!< allocated bytes

    private:
        void        *workspace; //!< allocated workspace
    };
    
}

#endif
