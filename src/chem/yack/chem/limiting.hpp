
//! \file

#ifndef YACK_CHEM_LIMITING_INCUDED
#define YACK_CHEM_LIMITING_INCUDED 1

#include "yack/chem/frontier.hpp"

namespace yack
{
    namespace chemical
    {


        //! lower limiting, with matching vanishing species
        class limiting : public frontier
        {
        public:
            explicit limiting(const sp_fund &) throw();
            virtual ~limiting() throw();

            void upgrade(const double   x,
                         const species &s);
            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(limiting);
        };


    }

}

#endif
