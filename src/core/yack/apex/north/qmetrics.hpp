//! \file

#ifndef YACK_APEX_NORTH_QMETRICS_INCLUDED
#define YACK_APEX_NORTH_QMETRICS_INCLUDED 1

#include "yack/apex.hpp"
#include "yack/container/writable.hpp"

namespace yack
{

    namespace north
    {
        //______________________________________________________________________
        //
        //
        //! orthogonal family maturity
        //
        //______________________________________________________________________
        enum maturity
        {
            in_progress, //! size < dims-1
            almost_done, //! size == dims-1, next vector is unique!
            fully_grown  //! size == dims
        };

        
        //______________________________________________________________________
        //
        //
        //! shared metrics for vectors and matrices
        //
        //______________________________________________________________________
        class qmetrics
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit qmetrics(const size_t dims); //!< setup, checked dimension
            virtual ~qmetrics() throw();          //!< cleanup
            qmetrics(const qmetrics &) throw();   //!< no-throw copy

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            static size_t   checked_dimension(const size_t); //!< check > 1
            static maturity updated_situation(const size_t dims,
                                              const size_t size) throw();  //!< update up to fully grown


#if 0
            //! prepare simplified univocal target from source and normSq = |target|^2
            static bool prepare_vector(writable<apq> &target,
                                       writable<apq> &source,
                                       apn           &normSq);
#endif
            
            //! prepare simplified univocal target from source and normSq = |target|^2
            static bool prepare_vector(writable<apz> &target,
                                       writable<apq> &source,
                                       apn           &normSq);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t dimension; //!< space dimension

        private:
            YACK_DISABLE_ASSIGN(qmetrics);
        };

    }

}

#endif

