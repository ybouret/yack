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
            static const char clid[];
            virtual ~qfamily() throw();
            
            //!
            /**
             \param rindx rows index, last is active (for use with roll)
             \param vbase base of vectors to use
             \param idxIO shared cache for indices
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
                const size_t nr = rindx.size(); assert(nr>0);
                const size_t ir = rindx[nr];    assert(ir<=vbase.rows);
                if(!qbase->grow(vbase[ir])) throw_invalid_init(ir);
                assign_all_indices(rindx);
            }

            friend std::ostream & operator<<(std::ostream &, const qfamily &);


            clone_ptr<qmatrix> qbase; //!< current qbase
            qidx_list          basis; //!< indices used to form qbase
            qidx_list          ready; //!< indices ready to be used
            qfamily           *next;  //!< for list
            qfamily           *prev;  //!< for list

            
        private:
            YACK_DISABLE_ASSIGN(qfamily);
            static void throw_invalid_init(const size_t ir);
            void        assign_all_indices(const readable<size_t> &rindx);

        };
        
    }
}

#endif

