//! \file 

#ifndef YACK_RAVEN_QVECTOR_INCLUDED
#define YACK_RAVEN_QVECTOR_INCLUDED 1

#include "yack/raven/qmetrics.hpp"
#include "yack/container/writable.hpp"

namespace yack
{
    namespace raven
    {
        //______________________________________________________________________
        //
        //
        //! vector of integers with their squared norm
        //
        //______________________________________________________________________
        class qvector : public qmetrics, public readable<apz>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit qvector(apz *ptr, const size_t num); //!< constructor
            virtual ~qvector()                   throw(); //!< cleanup

            //__________________________________________________________________
            //
            // interface: readable
            //__________________________________________________________________
            virtual size_t      size()                   const throw(); //!< dimension
            virtual const apz & operator[](const size_t) const throw(); //!< 1..dimension

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            friend std::ostream & operator<<(std::ostream &, const qvector &); //!< display
            void   ldz() throw();                                              //!< fast load of zero into coeff/norm2

            //! remove projection of v_k to u_k
            /**
             \param u_k current rational vector, initially v_k
             \param v_k trial vector
             */
            void sub(writable<apq>       &u_k,
                     const readable<apz> &v_k);

            //! hard copy all values
            void hard_copy(const qvector &other);

            //! try to build a univocal vector, return false if u_k==0
            bool appointed(const readable<apq> &u_k);

        private:
            apz      *coeff; //!< [1..dimension]

        public:
            const apn norm2; //!< |this|^2

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(qvector);
        };
    }

}

#endif
