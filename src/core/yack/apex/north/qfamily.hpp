//! \file

#ifndef YACK_NORTH_QFAMILY_INCLUDED
#define YACK_NORTH_QFAMILY_INCLUDED 1

#include "yack/apex/north/qmatrix.hpp"
#include "yack/ptr/clone.hpp"
#include "yack/data/small/set.hpp"

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

