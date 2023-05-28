
//! \file

#ifndef YACK_CHEM_VAT_CLUSTER_INCLUDED
#define YACK_CHEM_VAT_CLUSTER_INCLUDED 1

#include "yack/chem/equilibrium.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/container/matrix.hpp"

namespace yack
{
    namespace Chemical
    {

        class Cluster : public spot_object, public EqRepo, public Gathering
        {
        public:
            static const char CLID[];

            explicit Cluster(Equilibrium &first);
            virtual ~Cluster() noexcept;

            bool recruits(const Equilibrium &) const noexcept;
            bool linkedTo(const Cluster     &) const noexcept;

            friend std::ostream & operator<<( std::ostream &, const Cluster &);

            void updateIO() noexcept;
            void finalize(const xmlog &xml);

            SpRepo           active;
            matrix<int>      Nu;
            matrix<unsigned> Qm;
            const size_t     Qr; //!< rank(Qm)
            
            Cluster    *next;
            Cluster    *prev;
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Cluster);
            void discoverLaws(const xmlog &);
        };

    }


}

#endif

