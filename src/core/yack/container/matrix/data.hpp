

#ifndef YACK_CONTAINER_MATRIX_DATA_INCLUDED
#define YACK_CONTAINER_MATRIX_DATA_INCLUDED 1

#include "yack/container/matrix/row.hpp"

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
        inline virtual ~matrix_data() throw() { line=0; head=0; }  //!< cleanup
        
       
        
    protected:
        inline explicit matrix_data() throw() : line(0), head(0) {} //!< setup
        row          *line;                                         //!< in [1..rows]
        mutable_type *head;                                         //!< first data
        
        //! no-throw swap
        void swap_data_with(matrix_data &other) throw()
        {
            cswap(line,other.line);
            cswap(head,other.head);
        }
        
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(matrix_data);
    };
    
}

#endif
