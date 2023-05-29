//! \file

#ifndef YACK_WOVEN_QBUILDER_INCLUDED
#define YACK_WOVEN_QBUILDER_INCLUDED 1

#include "yack/woven/qbranch.hpp"
#include "yack/system/wtime.hpp"

namespace yack
{
    namespace woven
    {
        //______________________________________________________________________
        //
        //
        //! iterative builder of successive qbranches
        //
        //______________________________________________________________________
        class qbuilder : public metrics, public indices::fund
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit qbuilder(const size_t dims); //!< initialize
            virtual ~qbuilder() noexcept;         //!< cleanup
            
            //__________________________________________________________________
            //
            // fill repository
            /**
             \param repo repository to fill
             \param data original matrix
             \param rank orignial rank
             \param load keep single rows in repository
             */
            //__________________________________________________________________
            template <typename T> inline
            uint64_t operator()(woven::zrepo    &repo,
                                const matrix<T> &data,
                                const size_t     rank,
                                const bool       load)
            {
                const uint64_t mark = wtime::ticks();
                parents.initialize(data,repo,load);

                while(parents.size)
                {
                    //std::cerr << "#parents: " << parents.size << std::endl;
                    lineage.generate(parents,data,rank,repo);
                    lineage.swap_with(parents);
                }
                lineage.release();
                repo.sort();
                return wtime::ticks() - mark;
            }

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(qbuilder);
            qbranch  parents;
            qbranch  lineage;
        };
    }

}

#endif

