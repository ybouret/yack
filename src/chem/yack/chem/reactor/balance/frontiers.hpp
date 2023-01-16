//! \file

#ifndef YACK_CHEMICAL_FRONTIERS_INCLUDED
#define YACK_CHEMICAL_FRONTIERS_INCLUDED 1

#include "yack/chem/reactor/balance/frontier.hpp"
#include "yack/sequence/cxx-series.hpp"

namespace yack {

    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! assembly of frontiers, ranked
        //
        //______________________________________________________________________
        class frontiers : public cxx_series<frontier,memory::dyadic>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            typedef cxx_series<frontier,memory::dyadic> base_type; //!< base class

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit frontiers(const size_t); //!< setup with capacity
            virtual ~frontiers()     throw(); //!< setup with capacity



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(frontiers);
        };

    }

}

#endif

