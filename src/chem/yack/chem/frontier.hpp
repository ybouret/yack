
//! \file

#ifndef YACK_CHEM_FRONTIER_INCUDED
#define YACK_CHEM_FRONTIER_INCUDED 1

#include "yack/chem/species.hpp"

namespace yack
{
    namespace chemical
    {

        class frontier : public sp_repo
        {
        protected:
            explicit frontier(const sp_fund &) throw();
        public:
            virtual ~frontier() throw();
            void     destroy() throw();

            friend std::ostream & operator<<(std::ostream &, const frontier &);

            double   xi;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(frontier);
        };


    }

}

#endif
