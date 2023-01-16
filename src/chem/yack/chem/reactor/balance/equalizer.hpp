
//! \file

#ifndef YACK_CHEMICAL_EQUALIZER_INCLUDED
#define YACK_CHEMICAL_EQUALIZER_INCLUDED 1

#include "yack/chem/reactor/balance/frontier.hpp"
#include "yack/chem/reactor.hpp"
#include "yack/sequence/cxx-series.hpp"

namespace yack {

    namespace chemical
    {
        class frontiers : public cxx_series<frontier>
        {
        public:
            typedef cxx_series<frontier> base_type;

            explicit frontiers(const size_t n) : base_type(n) {}
            virtual ~frontiers() throw() {}

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(frontiers);
        };


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
