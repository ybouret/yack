//! \file

#ifndef YACK_APEX_ALGA_INCLUDED
#define YACK_APEX_ALGA_INCLUDED 1

#include "yack/apex.hpp"
#include "yack/container/matrix.hpp"

namespace yack
{


    //______________________________________________________________________
    //
    //
    //! Apex Library of Generic Algorithm
    //
    //______________________________________________________________________
    struct alga
    {

        //______________________________________________________________________
        //
        //! find least common multiple of an array of rationals
        //______________________________________________________________________
        template <typename ARRAY, typename INDEX> static inline
        apn lcm(ARRAY &arr, const INDEX lo, const INDEX hi)
        {
            assert(lo<=hi);
            if(lo>=hi)
                return arr[lo].den;
            else
            {
                apn res = apn::lcm_(arr[lo].den,arr[lo+1].den);
                for(INDEX i=lo+2;i<=hi;++i)
                    res = apn::lcm_(res,arr[i].den);
                return res;
            }
        }




        //______________________________________________________________________
        //
        // algorithm for rationals
        //______________________________________________________________________
        static void simplify( writable<apq> &v );                           //!< simplify keeping signs
        static void univocal( writable<apq> &v );                           //!< simplify to univocal
        static bool colinear(const readable<apq> &, const readable<apq> &); //!< check proportional
        
        //______________________________________________________________________
        //
        // algorithm for integers
        //______________________________________________________________________
        static void simplify( writable<apz> &v );                                 //!< simplify keeping signs
        static void univocal( writable<apz> &v );                                 //!< simplify to univocal
        static void definite( writable<apz> &v, apn &z2);                         //!< simplify to univocal with norm squared
        static bool colinear(const readable<apz> &lhs, const readable<apz> &rhs); //!< check proportional
        static bool colinear(const readable<int> &, const readable<int> & );      //!< test colinearity for ints


        //______________________________________________________________________
        //
        //! rank by gauss jordan
        //______________________________________________________________________
        static size_t rank_of(matrix<apq> &Q);

        //______________________________________________________________________
        //
        //! rank by gauss jordan of a matrix converted to apq
        //______________________________________________________________________
        template <typename T> static inline
        size_t  rank(const matrix<T> &M)
        {
            matrix<apq> Q(M,transmogrify);
            assert(matrix_metrics::have_same_sizes(Q,M));
            return  rank_of(Q);
        }

        //! simplify all rows
        template <typename T>
        static inline void simplify_rows(matrix<T> &M) {
            for(size_t i=M.rows;i>0;--i) simplify(M[i]);
        }

        //! univocla all rows
        template <typename T>
        static inline void univocal_rows(matrix<T> &M) {
            for(size_t i=M.rows;i>0;--i) univocal(M[i]);
        }



    };



}

#endif
