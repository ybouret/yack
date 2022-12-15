//! \file

#ifndef YACK_RAVEN_QMATRIX_INCLUDED
#define YACK_RAVEN_QMATRIX_INCLUDED 1

#include "yack/raven/qvector.hpp"
#include "yack/memory/shelf.hpp"
#include "yack/ptr/contractor.hpp"

namespace yack
{
    namespace raven
    {

        class qmatrix : public qmetrics, public readable<qvector>
        {
        public:
            virtual ~qmatrix() throw();
            explicit qmatrix(const size_t sz, const size_t rk);

            virtual size_t         size()                   const throw(); //!< current_rank
            virtual const qvector &operator[](const size_t) const throw(); //!< [1..current_rank]

            const size_t maximum_rank; //!< maximum rank
            const size_t current_rank; //!< current rank

        private:
            YACK_DISABLE_ASSIGN(qmatrix);

            memory::shelf       lib; //!< all linear memory
            contractor<apz>     obj; //!< maximum_rank * dimension + dimension for G-S
            contractor<qvector> row; //!< maximum_rank
            contractor<apq>     vgs; //!< dimension for G-S

            void initialize();

        };

    }

}

#endif
