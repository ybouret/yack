//! \file Right Angle Vectors ENdeavor

#ifndef YACK_RAVEN_QMETRICS_INCLUDED
#define YACK_RAVEN_QMETRICS_INCLUDED 1

#include "yack/apex.hpp"

namespace yack
{
    namespace raven
    {

        //! metrics elements for vectors and matrices
        class qmetrics
        {
        public:
            const size_t dimension;

            explicit qmetrics(const size_t dims); //!< dims>=2
            virtual ~qmetrics() throw();
            qmetrics(const qmetrics &) throw();

        private:
            YACK_DISABLE_ASSIGN(qmetrics);
        };

    }
}

#endif

