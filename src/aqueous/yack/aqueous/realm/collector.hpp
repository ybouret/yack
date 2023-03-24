
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
        typedef cxx_array<collecting::chart,memory::dyadic> collector_;

        //______________________________________________________________________
        //
        //
        //! collector for invalid concentrations
        //
        //______________________________________________________________________
        class collector :
        public collecting::caches,
        public collector_
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
            virtual ~collector() noexcept;      //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________



            //! settle all frontiers for each defined
            void probe(const realm       &chem,
                       writable <double> &conc);


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            eq_repo                      balanced; //!< fully balanced
            eq_repo                      solvable; //!< one side is bad but self-consisten
            eq_repo                      weakened; //!< one side  is  bad
            eq_repo                      singular; //!< two sides are bad
            cxx_array<double>            gain;     //!< store gains
            cxx_array<double>            Cend;     //!< combination
            matrix<double>               Cbal;     //!< store balanced concentrations
            cameo::add<double>           xadd;     //!< perform gain computation

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(collector);

            //! cleanup
            void initialize() noexcept;

            //! adjust according defined equilibrium
            /**
             \param xml       for output
             \param fmt       equilibria for output
             \param eqs       list of defined equilibria
             \param C         global concentrations
             \param R         global regular flags
             \param retaking partition of independent equilibria
             */
            void adjust(const xmlog            &xml,
                        const gathering        &fmt,
                        const eq_list          &eqs,
                        writable<double>      &C,
                        const readable<bool>   &R,
                        const partition        &retaking);

            double         combined(const cluster &);
            const cluster &find_opt(const partition        &part,
                                    const eq_repo          &zeqs,
                                    const xmlog            &xml);
            

            void   displace(writable<double> &C,
                          const cluster    &W);

        };

    }

}

#endif

