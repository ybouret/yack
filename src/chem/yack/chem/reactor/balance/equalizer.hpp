
//! \file

#ifndef YACK_CHEMICAL_EQUALIZER_INCLUDED
#define YACK_CHEMICAL_EQUALIZER_INCLUDED 1

#include "yack/chem/reactor/balance/frontier.hpp"
#include "yack/chem/reactor.hpp"

namespace yack {

    namespace chemical
    {

        class equalizer : public sp_fund
        {
        public:
            virtual ~equalizer() throw();
            explicit equalizer();

            frontier       fence;  //!< single fence

            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(equalizer);
        };

    }

}

#endif
