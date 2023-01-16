
//! \file

#ifndef YACK_CHEMICAL_EQUALIZER_INCLUDED
#define YACK_CHEMICAL_EQUALIZER_INCLUDED 1

#include "yack/chem/reactor/balance/frontier.hpp"
#include "yack/chem/reactor.hpp"

namespace yack {

    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! equalizing algorithm
        //
        //______________________________________________________________________
        class equalizer : public sp_fund
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~equalizer() throw();        //!< cleanup
            explicit equalizer(const reactor &); //!< setup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! adjust [reac|prod]_only using single fence
            void adjust(writable<double> &C);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const reactor &cs; //!< persistent reactor
            frontier       sf; //!< single fence


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(equalizer);
            void adjust(writable<double> &C, const cluster &cc);
            void adjust_reac(writable<double> &C, const eq_group  &reac_only);
            void adjust_prod(writable<double> &C, const eq_group  &prod_only);
         };

    }

}

#endif
