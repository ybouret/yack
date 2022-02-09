//! \file

#ifndef YACK_CHEM_PLEXUS_INCLUDED
#define YACK_CHEM_PLEXUS_INCLUDED 1

#include "yack/chem/equilibria.hpp"
#include "yack/chem/library.hpp"
#include "yack/sequence/arrays.hpp"
#include "yack/container/matrix.hpp"
#include "yack/apex.hpp"
#include "yack/math/algebra/lu.hpp"

namespace yack
{
    namespace chemical
    {

        typedef matrix<double> rmatrix;
        typedef matrix<int>    imatrix;

        //______________________________________________________________________
        //
        //
        //! system operation
        //
        //______________________________________________________________________
        class plexus
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef arrays_of<double>    tableaux;   //!< alias
            typedef tableaux::array_type array_type; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~plexus() throw();                //!< cleanup
            explicit plexus(library &, equilibria &); //!< setup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void   computeK(const double t);                              //!< pre-compute constants
            void   computeGamma(const readable<double> &C)       throw(); //!< evaluate mass action
            void   computeGammaAndPsi(const readable<double> &C) throw(); //!< evaluate mass action and jacobian
            double objectiveGamma() throw();
            
            //! solve
            void solve(writable<double> &C);

            double operator()(const double);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const library    &lib;  //!< support library
            const equilibria &eqs;  //!< support equilibria
            const size_t      N;    //!< equilibria count
            const size_t      M;    //!< species count
            const size_t      A;    //!< active species

        private:
            tableaux ntab;
            tableaux mtab;
        public:
            array_type       &K;      //!< [N] constants at a given time
            array_type       &Gamma;  //!< [N] Gamma for precomputed K
            array_type       &Corg;   //!< [M] initial concentrations
            array_type       &Ctry;   //!< [M] trial concentrations
            array_type       &dC;     //!< [M] delta C
            array_type       &xi;     //!< [N] xi
            const imatrix     Nu;     //!< topology
            const imatrix     NuT;    //!< Nu'
            rmatrix           Psi;    //!< jacobian of Gamm
            rmatrix           Omega;  //!< Omega
            vector<double>    rstack; //!< temporary stack of real
            vector<size_t>    ustack; //!< temporary stack of unsigned
            math::lu<double>  LU;     //!< [N]

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(plexus);
            const lockable::scope lib_lock;
        };
        
    }
}

#endif

