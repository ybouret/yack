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
        class Vat : public Clusters
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            explicit Vat(const xmlog      &xml,
                         Equilibria       &eqs,
                         Extended::Vector &usr);

            //! cleanup
            virtual ~Vat() noexcept;

            void updateK( Extended::Vector &K, const double t);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t maxClusterSize; //!< maximum number of equilibria in a cluster
            const size_t maximumSpecies; //!< maximum number of active species in a cluster


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Vat);
            void makeClusters(const xmlog &, Equilibria &, Extended::Vector &);
        };
    }

}

#endif

