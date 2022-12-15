//! \file 

#ifndef YACK_RAVEN_QVECTOR_INCLUDED
#define YACK_RAVEN_QVECTOR_INCLUDED 1

#include "yack/raven/qmetrics.hpp"
#include "yack/container/writable.hpp"

namespace yack
{
    namespace raven
    {

        class qvector : public qmetrics, public readable<apz>
        {
        public:
            explicit qvector(apz *ptr, const size_t num);
            virtual ~qvector() throw();

            virtual size_t      size()                   const throw();
            virtual const apz & operator[](const size_t) const throw();

            friend std::ostream & operator<<(std::ostream &, const qvector &);

            //! remove projection of v_k to u_k
            /**
             \param u_k current rational vector, initially v_k
             \param v_k trial vector
             */
            void sub(writable<apq>       &u_k,
                     const readable<apz> &v_k);

            void normalize();
            bool appointed(const readable<apq> &u_k);

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
