
//! \file

#ifndef YACK_CHEM_VAT_CLUSTER_INCLUDED
#define YACK_CHEM_VAT_CLUSTER_INCLUDED 1

#include "yack/chem/equilibria.hpp"
#include "yack/chem/vat/canon.hpp"
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
            void finalize(const xmlog &xml, Equilibria &eqs);

            //! display components
            friend std::ostream & operator<<( std::ostream &, const Cluster &);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const SpList           lib;     //!< employed species
            const matrix<int>      Nu;      //!< main topology
            const matrix<unsigned> Qmat;    //!< matrix of constraints
            const size_t           Qrnk;    //!< rank(Qm)
            const ConservationLaws laws;    //!< effective laws
            const Canons           acts;    //!< group of dependent laws
            
            Cluster               *next;    //!< for list
            Cluster               *prev;    //!< for list
            Gathering              reac;    //!< to display reac
            Gathering              prod;    //!< to display prod
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Cluster);
            void discoverLaws(const xmlog &);
            void assembleActs(const xmlog &);
            void makeManifold(const xmlog &, Equilibria &eqs);
        };

    }


}

#endif

