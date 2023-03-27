
//! \file

#ifndef YACK_AQUEOUS_COLLECTOR_INCLUDED
#define YACK_AQUEOUS_COLLECTOR_INCLUDED 1

#include "yack/aqueous/realm/domain.hpp"
#include "yack/aqueous/realm/coll/chart.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/data/dinky/solo-repo.hpp"


namespace yack
{
    namespace aqueous
    {

        class realm;
        
        //______________________________________________________________________
        //
        //
        //! base class for collector
        //
        //______________________________________________________________________
        typedef cxx_array<collecting::chart,memory::dyadic> charts_type;

        //______________________________________________________________________
        //
        //
        //! collector for invalid concentrations
        //
        //______________________________________________________________________
        class collector :  public cxx_array<double>, public collecting::caches
        {
        public:
            //__________________________________________________________________
            //
            // definition
            //__________________________________________________________________
            typedef collecting::chart chart; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit collector(const size_t n, const size_t m); //!<  setup
            virtual ~collector() noexcept;                      //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________


            //! settle all frontiers for each defined
            void balance(const realm       &chem,
                         writable <double> &conc);



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(collector);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            charts_type                  charts;   //!< all charts
            eq_repo                      balanced; //!< fully balanced
            eq_repo                      solvable; //!< one side is bad but self-consisten
            eq_repo                      weakened; //!< one side  is  bad
            eq_repo                      singular; //!< two sides are bad
            boundaries                   solo;     //!< for splitting/combining
            cxx_array<double>            gain;     //!< store gains per equilibrium
            matrix<double>               Cbal;     //!< [NxM]store balanced concentrations
            cameo::add<double>           xadd;     //!< perform gain computation

        private:
            //! cleanup
            void initialize() noexcept;

            //! adjust according defined equilibrium
            void balance(const xmlog            &xml,
                         const domain           &dom,
                         writable<double>       &C);



            //! return true is shift_status != blocked
            bool           dispatch(const equilibrium &, const shift_status);

            //! get gain of combined solutions
            double         combined(const cluster &);

            //! find cluster with best gain
            const cluster &find_opt(const partition        &part,
                                    const eq_repo          &zeqs,
                                    const xmlog            &xml);
            
            //! compute new concentration from winning clusster
            void   displace(writable<double> &C, const cluster    &W);

            //! check if some fixing was needed for roaming species
            bool   needed_some_fixing(const actors &A, writable<double> &C);


        };

    }

}

#endif

