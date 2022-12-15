
//! \file Right Angle Vectors ENdeavor

#ifndef YACK_RAVEN_QVECTOR_INCLUDED
#define YACK_RAVEN_QVECTOR_INCLUDED 1

#include "yack/raven/qmetrics.hpp"

namespace yack
{
    namespace raven
    {

        class qvector : public qmetrics, public readable<apz>
        {
        public:
            explicit qvector(apz *ptr, const size_t num);
            virtual ~qvector() throw();

        private:
            apz      *coeff; //!< [1..dimension]

        public:
            const apn norm2;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(qvector);
        };
    }

}

#endif
