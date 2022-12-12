//! \file

#ifndef YACK_APEX_NORTH_CONSTELLATION_INCLUDED
#define YACK_APEX_NORTH_CONSTELLATION_INCLUDED 1

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
        //! data management
        //
        //______________________________________________________________________
        struct constellation
        {
            static size_t   checked_dimension(const size_t);               //!< check positive
            static maturity initial_situation(const size_t);               //!< initial satus
            static maturity updated_situation(const size_t dims,
                                              const size_t size) throw();  //!< update up to fully grown

            //! check orthogonality
            static bool are_orthogonal(const readable<apq> &lhs,
                                       const readable<apq> &rhs);

            //! check orthogonality
            static bool are_orthogonal(const readable<apq>     &lhs,
                                       const readable<int64_t> &rhs);

            //! prepare simplified univocal target from source and normSq = |target|^2
            static bool prepare_vector(writable<apq> &target,
                                       writable<apq> &source,
                                       apn           &normSq);

            //! prepare simplified univocal target from source and normSq = |target|^2
            static bool prepare_vector(writable<int64_t> &target,
                                       writable<apq>     &source,
                                       uint64_t          &normSq);

        };

        //! helper for type selection
        template <typename T> struct classify;

        //! type selection for apq
        template < > struct classify<apq>
        {
            typedef apq     type;    //!< alias
            typedef apn     l2_type; //!< alias

            template <typename U> static inline
            void xch(U &lhs, U &rhs) throw() { lhs.xch(rhs); }
            
        };

        //! type selection for int64_t
        template < > struct classify<int64_t>
        {
            typedef int64_t  type;    //!< alias
            typedef uint64_t l2_type; //!< alias

            template <typename U> static inline
            void xch(U &lhs, U &rhs) throw() { cswap(lhs,rhs); }
        };

        class qmetrics
        {
        public:
            const size_t dimension;

            explicit qmetrics(const size_t dims);
            virtual ~qmetrics() throw();
            qmetrics(const qmetrics &) throw();

        private:
            YACK_DISABLE_ASSIGN(qmetrics);
        };

    }

}

#endif

