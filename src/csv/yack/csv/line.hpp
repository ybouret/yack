//! \file

#ifndef YACK_CSV_LINE_INCLUDED
#define YACK_CSV_LINE_INCLUDED

#include "yack/csv/cell.hpp"
#include "yack/data/list/cxx.hpp"


namespace yack
{

    namespace CSV
    {


        //______________________________________________________________________
        //
        //
        //! list of Cells for Line
        //
        //______________________________________________________________________
        typedef cxx_list_of<Cell> Cells;


        //______________________________________________________________________
        //
        //
        //! Line of Cells
        //
        //______________________________________________________________________
        class Line : public Cells
        {
        public:
            //__________________________________________________________________
            //
            //  C++
            //__________________________________________________________________
            explicit Line() noexcept; //!< setup empty
            virtual ~Line() noexcept; //!< cleanup
            Line(const Line &);       //!< copy
            
            //__________________________________________________________________
            //
            //  members
            //__________________________________________________________________
            Line *next; //!< for list
            Line *prev; //!< for list

        private:
            YACK_DISABLE_ASSIGN(Line);
        };

    }

}


#endif

