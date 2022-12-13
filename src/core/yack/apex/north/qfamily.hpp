//! \file

#ifndef YACK_NORTH_QFAMILY_INCLUDED
#define YACK_NORTH_QFAMILY_INCLUDED 1

#include "yack/apex/north/qmatrix.hpp"
#include "yack/ptr/clone.hpp"
#include "yack/data/small/set.hpp"
#include "yack/container/matrix.hpp"

namespace yack
{
    namespace north
    {
        typedef small_set<size_t>    qidx_list;
        typedef qidx_list::bank_type qidx_bank;
        typedef qidx_list::bank_ptr  qidx_bptr;
        
        class qfamily : public object
        {
        public:
            virtual ~qfamily() throw();
            
            //!
            /**
             \param rindx rows index, last is active (for use with roll)
             \param vbase base of vectors to use
             */
            template <typename T>
            inline   qfamily(const readable<size_t> &rindx,
                             const matrix<T>        &vbase,
                             const qidx_bptr        &idxIO) :
            qbase( new qmatrix(vbase.cols) ),
            basis(idxIO),
            ready(idxIO),
            next(NULL),
            prev(NULL)
            {
            }
            
            clone_ptr<qmatrix> qbase; //!< current qbase
            qidx_list          basis; //!< indices used to form qbase
            qidx_list          ready; //!< indices ready to be used
            qfamily           *next;  //!< for list
            qfamily           *prev;  //!< for list

            
        private:
            YACK_DISABLE_ASSIGN(qfamily);
        };
        
    }
}

#endif

