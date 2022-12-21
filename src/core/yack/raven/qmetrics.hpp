//! \file

#ifndef YACK_RAVEN_QMETRICS_INCLUDED
#define YACK_RAVEN_QMETRICS_INCLUDED 1

#include "yack/apex.hpp"

namespace yack
{
    // Right Angle Vectors ENdeavor
    namespace raven
    {
        //______________________________________________________________________
        //
        //
        //! metrics elements for vectors and matrices
        //
        //______________________________________________________________________
        class qmetrics
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit qmetrics(const size_t dims); //!< dims>=2
            virtual ~qmetrics()          throw(); //!< cleanup
            qmetrics(const qmetrics &)   throw(); //!< copy

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t dimension; //!< commpm dimension

        private:
            YACK_DISABLE_ASSIGN(qmetrics);
        };

    }
}

#endif

