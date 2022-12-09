
//! \file

#ifndef YACK_APEX_NORTH_CONSTELLATION_INCLUDED
#define YACK_APEX_NORTH_CONSTELLATION_INCLUDED 1

#include "yack/apex.hpp"
#include "yack/container/writable.hpp"

namespace yack
{

    namespace north
    {
        //! family maturity
        enum maturity
        {
            in_progress, //! size < dims-1
            almost_done, //! size == dims-1, next vector is unique!
            fully_grown  //! size == dims
        };

        struct constellation
        {
            static size_t   checked_dimension(const size_t d);
            static maturity initial_situation(const size_t dims);
            static maturity updated_situation(const size_t dims, const size_t size) throw();

            //! check orthogonality
            static bool are_orthogonal(const readable<apq> &lhs,
                                       const readable<apq> &rhs);

            //! prepare simplified univocal target from source and normSq = |target|^2
            static bool prepare_vector(writable<apq> &target,
                                       writable<apq> &source,
                                       apn           &normSq);

        };

        template <typename T> struct classify;

        template < > struct classify<apq>
        {
            typedef apq     type;
            typedef apn     l2_type;
        };

        template < > struct classify<int64_t>
        {
            typedef int64_t  type;
            typedef uint64_t l2_type;
        };

    }

}

#endif

