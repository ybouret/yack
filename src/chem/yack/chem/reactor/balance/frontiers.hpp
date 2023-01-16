//! \file

#ifndef YACK_CHEMICAL_FRONTIERS_INCLUDED
#define YACK_CHEMICAL_FRONTIERS_INCLUDED 1

#include "yack/chem/reactor/balance/frontier.hpp"
#include "yack/sequence/cxx-series.hpp"

namespace yack {

    namespace chemical
    {

        class frontiers : public cxx_series<frontier,memory::dyadic>
        {
        public:
            typedef cxx_series<frontier,memory::dyadic> base_type;

            explicit frontiers(const size_t);
            virtual ~frontiers()     throw();



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(frontiers);
        };

    }

}

#endif

