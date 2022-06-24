//! \file

#ifndef YACK_CHEM_PLEXUS_INCLUDED
#define YACK_CHEM_PLEXUS_INCLUDED 1

#include "yack/chem/library.hpp"
#include "yack/chem/equilibria.hpp"
#include "yack/container/matrix.hpp"
#include "yack/math/algebra/lu.hpp"
#include "yack/sequence/arrays.hpp"
#include "yack/chem/clusters.hpp"

namespace yack
{
    namespace chemical
    {


        typedef matrix<int>            imatrix;  //!< alias
        typedef matrix<double>         rmatrix;  //!< alias
        typedef vector<double>         rvector;  //!< alias
        typedef arrays_of<double>      tableaux; //!< alias
        typedef tableaux::array_type   tableau;  //!< alias
        typedef thin_array<bool>       booltab;  //!< alias


        //______________________________________________________________________
        //
        //
        //! chemical plexus
        //
        //______________________________________________________________________
        class plexus
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char  clid[];  //!< "chemical::plexus"
            static const bool &verbose; //!< reference on entity::verbose
            static const char  vpfx[];  //!< prefix for verbosity

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! initialize from lib/eqs at tini to compute initial constants
            explicit plexus(library     &lib_,
                            equilibria  &eqs_,
                            const double tini);
            virtual ~plexus() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! compute all singles and couples constants
            void computeK(const double t);

            //! solve
            bool solve( writable<double> &C0 ) throw();

            //! transfert only active components
            template <typename T, typename U> inline
            void transfer( writable<T> &target, const readable<U> &source) const
            {
                for(const anode *node=active.head;node;node=node->next)
                {
                    const size_t j = ***node;
                    target[j] = static_cast<T>(source[j]);
                }
            }

            double hamiltonian(const readable<double> &C); //!< |Gamma|^2/N 
            double operator()(const double u);             //!< hamiltonian( (1-u) * Corg + u * Cend )


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const library    &lib;     //!< user's library of species
            const library     sub;     //!< duplicate library to build database
            const equilibria &singles; //!< user's equilibria
            const equilibria  couples; //!< all possible couples
            const equilibria  lattice; //!< singles+couples
            const size_t      M;       //!< number of species
            const size_t      N;       //!< number of equilibria
            const size_t      Nc;      //!< number of couples
            const size_t      Nl;      //!< lattice dimension
            
        private:
            tableaux          ntab;    //!< tableaux for size() = N
            tableaux          mtab;    //!< tableaux for size() = M
            tableaux          ltab;    //!< tableaux for size() = L

        public:
            const alist       active;  //!< active species list
            const imatrix     Nu;      //!< [NxM] topology
            const imatrix     NuT;     //!< [MxN] Nu'
            tableau          &K;       //!< [N] constants of singles
            tableau          &Xtry;    //!< [N] temporary

            tableau          &Corg;    //!< [M] workspace
            tableau          &Ctry;    //!< [M] workspace
            tableau          &Cend;    //!< [M] workspace

            tableau          &Kl;      //!< [Nl] constants of lattice
            tableau          &Xl;      //!< [Nl] all extents
            rmatrix           Cs;      //!< [NlxM] all solving concentrations

            const clusters    com;     //!< clusters of detached combinations
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(plexus);
            const lockable::scope libLock;
            const lockable::scope eqsLock;

            size_t buildMatchingCouples();
            void   duplicateIntoLattice(const equilibria &);
            double computeLatticeExtent();
            void   makeReactiveClusters();
        };

    }

}

#endif

