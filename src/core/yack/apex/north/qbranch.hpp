//! \file

#ifndef YACK_NORTH_QBRANCH_INCLUDED
#define YACK_NORTH_QBRANCH_INCLUDED 1

#include "yack/apex/north/qfamily.hpp"

namespace yack
{
    namespace north
    {

        class qbranch : public qfamily::list_type
        {
        public:
            explicit qbranch() throw();
            virtual ~qbranch() throw();

            friend std::ostream & operator<<(std::ostream &, const qbranch &);
            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(qbranch);
        };

    }

}

#endif
