
//! \file

#ifndef YACK_CHEMICAL_EQUALIZER_INCLUDED
#define YACK_CHEMICAL_EQUALIZER_INCLUDED 1

#include "yack/chem/reactor/balance/frontier.hpp"
#include "yack/chem/reactor.hpp"
#include "yack/sequence/cxx-series.hpp"

namespace yack {

    namespace chemical
    {
        class frontiers : public cxx_series<frontier,memory::dyadic>
        {
        public:
            typedef cxx_series<frontier,memory::dyadic> base_type;

            explicit frontiers(const size_t n) : base_type(n) {}
            virtual ~frontiers() throw() {}

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(frontiers);
        };

        class boundaries
        {
        public:
            explicit boundaries(const size_t m) :
            limiting(m),
            amending(m)
            {
            }


            virtual ~boundaries() throw() {}

            frontiers limiting; //!< from positive concentration
            frontiers amending; //!< from negative concentration

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(boundaries);
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
            const reactor &cs;    //!< persistent reactor
            const size_t   mx;    //!< max actors
            frontier       sf;    //!< single fence
            boundaries     reac;  //!< boundaries from reactant
            boundaries     prod;  //!< boundaries from products
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(equalizer);
            void adjust(writable<double> &C, const cluster &cc);
            void adjust_reac(writable<double> &C, const eq_group  &reac_only);
            void adjust_prod(writable<double> &C, const eq_group  &prod_only);
         };

    }

}

#endif
