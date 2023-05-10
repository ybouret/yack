
//! \file

#ifndef YACK_CSV_DOC_INCLUDED
#define YACK_CSV_DOC_INCLUDED

#include "yack/csv/line.hpp"


namespace yack
{

    namespace CSV
    {

        //______________________________________________________________________
        //
        //
        //! list of Lines for Document
        //
        //______________________________________________________________________
        typedef cxx_list_of<Line> Lines;


        //______________________________________________________________________
        //
        //
        //! list of Lines
        //
        //______________________________________________________________________
        class Document : public Lines
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Document() noexcept; //!< setup empty
            virtual ~Document() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! display
            friend std::ostream & operator<<(std::ostream &os, const Document &doc);

        private:
            YACK_DISABLE_ASSIGN(Document);
        };
    }

}

#endif

