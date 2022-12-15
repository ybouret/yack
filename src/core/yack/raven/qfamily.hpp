//! \file

#ifndef YACK_RAVEN_QFAMILY_INCLUDED
#define YACK_RAVEN_QFAMILY_INCLUDED 1

#include "yack/raven/qmatrix.hpp"


namespace yack
{
    namespace raven
    {

        class qfamily : public object
        {
        public:
            virtual ~qfamily() throw();


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(qfamily);
        };

    }

}

#endif

