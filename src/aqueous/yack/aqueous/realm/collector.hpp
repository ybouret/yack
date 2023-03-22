
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
            /**
             \param xml for output
             \param fmt for output
             \param eqs list of defined equilibria
             \param C global concentrations
             \param R global regular flags
             */
            void probe(const xmlog            &xml,
                       const gathering        &fmt,
                       const eq_list          &eqs,
                       const readable<double> &C,
                       const readable<bool>   &R);

            void probe(const realm       &chem,
                       writable <double> &conc);


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            solo_repo<const equilibrium> unbal; //!< list of unbalanced equilibria
            matrix<double>               Cbal;  //!< store balanced     concentrations
            cxx_array<bool>              good;  //!< status of balanced concentrations
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(collector);
            
        };

    }

}

#endif

