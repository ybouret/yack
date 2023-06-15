//! \file

#ifndef YACK_CHEM_VAT_INCLUDED
#define YACK_CHEM_VAT_INCLUDED 1

#include "yack/chem/vat/cluster.hpp"
#include "yack/chem/library.hpp"
#include "yack/chem/equilibria.hpp"

namespace yack
{
    namespace Chemical
    {
        typedef cxx_list_of<Cluster> Clusters; //!< alias

        //______________________________________________________________________
        //
        //
        //! Vat of equilibria
        //
        //______________________________________________________________________
        class Vat : public Equilibria, public Clusters
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            /**
             - copy equilibria
             - make clusters from dependents equilibria
             - make manifolds
             */
            explicit Vat(const xmlog      &xml,
                         const Equilibria &eqs);

            //! cleanup
            virtual ~Vat() noexcept;

            //! update all constants, in order
            void updateK(const double t);

            //! display equilibria+constant per cluster
            void displayK(std::ostream &) const;

            //! display equilibria+scaling per cluster
            void displayS(std::ostream &) const;


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Extended::Vector    K; //!< constants
            const Extended::Vector    S; //!< scaling factor for one way only equilibria
            const size_t maxClusterSize; //!< maximum number of equilibria in a cluster
            const size_t maximumSpecies; //!< maximum number of active species in a cluster


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Vat);
            void makeClusters(const xmlog &, Equilibria &, Extended::Vector &);
        };
    }

}

#endif

