//! \file

#ifndef YACK_CHEM_PLEXUS_INCLUDED
#define YACK_CHEM_PLEXUS_INCLUDED 1

#include "yack/chem/equilibria.hpp"
#include "yack/chem/library.hpp"
#include "yack/sequence/arrays.hpp"
#include "yack/container/matrix.hpp"
#include "yack/apex.hpp"

namespace yack
{
    namespace chemical
    {
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
            void computeK(const double t);                              //!< pre-compute constants
            void computeGamma(const readable<double> &C)       throw(); //!< evaluate mass action
            void computeGammaAndPsi(const readable<double> &C) throw(); //!< evaluate mass action and jacobian
            void computeOmega() throw();                                //!< from Gamma and NuT

            void study();

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
            array_type       &K;      //!< constants at a given time
            array_type       &Gamma;  //!< Gamma for precomputed K
            const matrix<int> Nu;     //!< topology
            const matrix<int> NuT;    //!< Nu'
            matrix<double>    Psi;    //!< jacobian of Gamm
            matrix<double>    Omega;  //!< Omega



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(plexus);
            const lockable::scope lib_lock;
        };
        
    }
}

#endif

