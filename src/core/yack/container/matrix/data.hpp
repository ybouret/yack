

#ifndef YACK_CONTAINER_MATRIX_DATA_INCLUDED
#define YACK_CONTAINER_MATRIX_DATA_INCLUDED 1

#include "yack/container/matrix/row.hpp"
#include "yack/large-object.hpp"
#include "yack/counted.hpp"

namespace yack
{

    namespace kernel
    {
        //______________________________________________________________________
        //
        //! base class for matrix_data
        //______________________________________________________________________
        class matrix_data : public large_object, public counted
        {
        public:    virtual ~matrix_data() noexcept; //!< cleanup
        protected: explicit matrix_data() noexcept; //!< setup
        private: YACK_DISABLE_COPY_AND_ASSIGN(matrix_data);
        };
    }

    //__________________________________________________________________________
    //
    //
    //! data for matrix
    //
    //__________________________________________________________________________
    template <typename T>
    class matrix_data : public kernel::matrix_data
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef matrix_row<T> row;                         //!< alias
        typedef typename row::mutable_type mutable_type;   //!< alias
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline virtual ~matrix_data() noexcept { line=0; head=0; }  //!< cleanup
        

        
    protected:
        inline explicit matrix_data() noexcept :
        kernel::matrix_data(), line(0), head(0) {} //!< setup
        row          *line;                                         //!< in [1..rows]
        mutable_type *head;                                         //!< first data
        
        //! no-throw swap
        void swap_data_with(matrix_data &other) noexcept
        {
            cswap(line,other.line);
            cswap(head,other.head);
        }
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(matrix_data);
    };
    
}

#endif
