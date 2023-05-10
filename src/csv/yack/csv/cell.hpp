
//! \file

#ifndef YACK_CSV_CELL_INCLUDED
#define YACK_CSV_CELL_INCLUDED

#include "yack/string.hpp"

namespace yack
{

    namespace CSV
    {

        //______________________________________________________________________
        //
        //
        //! cell for CSV
        //
        //______________________________________________________________________
        class Cell : public string
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________

            //! type of cell
            enum Type
            {
                isRaw, //!< raw content
                isStr  //!< stringified content
            };

            //__________________________________________________________________
            //
            //  C++
            //__________________________________________________________________

            //! cell with data
            template <typename DATA> inline
            explicit Cell(const DATA &s, const Type t) : string(s), type(t), next(0), prev(0) {}
            explicit Cell();          //!< empty raw cell
            virtual ~Cell() noexcept; //!< cleanup
            Cell(const Cell &);       //!< copy

            //__________________________________________________________________
            //
            //  members
            //__________________________________________________________________
            Type  type; //!< type
            Cell *next; //!< for list
            Cell *prev; //!< for list

        private:
            YACK_DISABLE_ASSIGN(Cell);

        };

    }

}

#endif
