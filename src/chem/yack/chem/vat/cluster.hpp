
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

        //______________________________________________________________________
        //
        //
        //! Cluster of interdependent equilibria
        //
        //______________________________________________________________________
        class Cluster : public spot_object, public EqList, public Gathering
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const char CLID[]; //!< "Cluster"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Cluster(Equilibrium &first); //!< setup with first equilibrium
            virtual ~Cluster() noexcept;          //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            bool recruits(const Equilibrium &) const noexcept; //!< true if equilibrium is link to one of self
            bool linkedTo(const Cluster     &) const noexcept; //!< true if at least two equilibria area linked

            //! finalize cluster
            /**
             - build and check topology and local indices
             - discoverLaws()
             - makeManifold()
             */
            void finalize(const xmlog &xml);

            //! display components
            friend std::ostream & operator<<( std::ostream &, const Cluster &);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const SpList           species; //!< employed specis
            const matrix<int>      Nu;      //!< main topology
            const matrix<unsigned> Qm;      //!< matrix of constraints
            const size_t           Qr;      //!< rank(Qm)
            Cluster               *next;    //!< for list
            Cluster               *prev;    //!< for list

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Cluster);
            void discoverLaws(const xmlog &);
            void makeManifold(const xmlog &);
        };

    }


}

#endif

