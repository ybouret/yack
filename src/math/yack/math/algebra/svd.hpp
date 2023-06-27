
//! \file

#ifndef YACK_MATH_SVD_INCLUDED
#define YACK_MATH_SVD_INCLUDED 1

#include "yack/container/matrix.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/ipso/add.hpp"

namespace yack
{
    namespace math
    {

        //______________________________________________________________________
        //
        //
        //! Singular Values Decomposition
        //
        //______________________________________________________________________
        template <typename T>
        class svd
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const size_t MAX_ITS = 1024; //!< maximum iterations
            typedef T                type;      //!< floating point
            typedef extended<type>   xtype;     //!< extended
            typedef ipso::add<xtype> add_type;  //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit svd(const size_t n=0); //!< setup with capacity
            virtual ~svd() noexcept;      //!< cleanup

            //__________________________________________________________________
            //
            // method
            //__________________________________________________________________

            //! singular values decomposition
            /**
             - Given a matrix a[1..m][1..n], this routine computes its singular value
             decomposition, a = u.w.v'.
             - The matrix u replaces a on output.
             - The diagonal matrix of singular values w is output as a vector w[1..n].
             - The matrix v (not the transpose v') is output as v[1..n][1..n].
             */
            bool build(matrix<xtype> &a, writable<xtype> &w, matrix<xtype> &v);

            //! singular value solve (u.w.v') x = b
            void solve(const  matrix<xtype>   &u,
                       const  readable<xtype> &w,
                       const  matrix<xtype>   &v,
                       writable<xtype>        &x,
                       const readable<xtype>  &b);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const xtype zero; //!< 0
            const xtype one;  //!< 1
            const xtype two;  //!< 2
            add_type    add;  //!< helper

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(svd);
            vector<xtype,memory::dyadic> rv1;   //!< a.cols=n
        };

    }
}

#endif
