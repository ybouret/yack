
//! \file

#ifndef YACK_CHEMICAL_BALANCING_INCLUDED
#define YACK_CHEMICAL_BALANCING_INCLUDED 1

#include "yack/chem/nexus.hpp"
#include "yack/type/authority.hpp"
#include "yack/chem/equalizer.hpp"

namespace yack
{

    namespace chemical
    {
       

        //______________________________________________________________________
        //
        //
        //! balancing algorithm
        //
        //______________________________________________________________________
        class balancing : public authority<const nexus>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit balancing(const nexus &, const xmlog &); //!< setup
            virtual ~balancing() throw();                     //!< cleanup
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            bool operator()(writable<double> &C0); //!< balance C0
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            sp_fund      spIO;        //!< shared
            equalizer    reac;
            equalizer    prod;
            rmatrix      Cbalanced;  //!< [LxM] balanced concentration matrix
            frontier     vanishing;  //!< result to compute balananced concentrations and score
            raddops      xadd;
            const xmlog &xml;   //!< helper

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(balancing);
            bool balance(writable<double> &C0,
                         const cluster    &cc);

            double score(const readable<double> &C0, const equilibrium &);

        };


    }

}

#endif

