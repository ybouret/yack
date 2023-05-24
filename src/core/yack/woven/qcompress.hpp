//! \file

#ifndef YACK_WOVEN_QCOMPRESS_INCLUDED
#define YACK_WOVEN_QCOMPRESS_INCLUDED 1

#include "yack/apex/flak.hpp"
#include "yack/data/dinky/core-repo.hpp"

namespace yack
{
    namespace woven
    {

        //______________________________________________________________________
        //
        //
        //! compressing input rows
        //
        //______________________________________________________________________
        struct qcompress
        {

            //__________________________________________________________________
            //
            //! count number of no zero coefficients
            //__________________________________________________________________
            template <typename ARRAY>
            static inline size_t neqz(ARRAY &arr)
            {
                size_t res=0;
                for(size_t i=arr.size();i>0;--i)
                {
                    if(0!=arr[i]) ++res;
                }
                return res;
            }

            //__________________________________________________________________
            //
            //! keeping only not null, not pairwise colinear vectors
            /**
             \param output valid entry matrix for qbuilder
             \param input  matrix of test rows
             */
            //__________________________________________________________________
            template <typename T, typename U> static inline
            size_t build(matrix<T>       &output,
                         const matrix<U> &input)
            {
                typedef readable<U>                  row_type;
                typedef core_repo<row_type>          row_repo;
                typedef typename row_repo::node_type row_node;

                //--------------------------------------------------------------
                //
                // initialize empty rows
                //
                //--------------------------------------------------------------
                row_repo rows;
                for(size_t i=1;i<=input.rows;++i)
                {
                    //----------------------------------------------------------
                    // take next input row
                    //----------------------------------------------------------
                    const row_type &rhs  = input[i];
                    if( neqz(rhs) <= 0 ) continue;

                    //----------------------------------------------------------
                    // check no colinearity with recorded rows
                    //----------------------------------------------------------
                    bool ok  = true;
                    for(const row_node *node=rows.head;node;node=node->next)
                    {
                        const row_type &lhs = ***node;
                        if(apex::flak::colinear(lhs,rhs))
                        {
                            ok = false;
                            break;
                        }
                    }

                    //----------------------------------------------------------
                    // record this row
                    //----------------------------------------------------------
                    if(ok) rows << rhs;
                }

                //--------------------------------------------------------------
                //
                // prepare and fill output
                //
                //--------------------------------------------------------------
                const size_t nr = rows.size;
                if(nr<=0)
                {
                    output.release();
                    return 0;
                }
                else
                {
                    const size_t nc = input.cols;
                    output.make(nr,nc);
                    size_t ir=1;
                    for(const row_node *node=rows.head;node;node=node->next,++ir)
                    {
                        writable<T>    &target = output[ir];
                        const row_type &source = ***node; assert(source.size()==target.size());
                        for(size_t j=nc;j>0;--j) target[j] = static_cast<T>(source[j]);
                    }
                    return apex::flak::rank(output);
                }
            }


        };
    }
}

#endif

