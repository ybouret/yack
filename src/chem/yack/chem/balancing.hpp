
//! \file

#ifndef YACK_CHEMICAL_BALANCING_INCLUDED
#define YACK_CHEMICAL_BALANCING_INCLUDED 1

#include "yack/chem/nexus.hpp"
#include "yack/type/authority.hpp"
#include "yack/chem/balancing/equalizer.hpp"
#include "yack/chem/eqs/repo.hpp"

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
            sp_fund      spIO;       //!< cache
            eq_fund      eqIO;       //!< cache
            equalizer    reac;       //!< compute reac pos/neg
            equalizer    prod;       //!< compute prod pos/neg
            rmatrix      Cbal;       //!< [LxM] balanced concentration matrix
            frontier     fade;       //!< result to compute balananced concentrations and score
            eq_repo      lead;       //!< possibilities
            tableau      gain;       //!< store gains
            raddops      xadd;       //!< for internal computation
            const xmlog &xml;        //!< helper

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(balancing);
            bool balance(writable<double> &C0,
                         const cluster    &cc);

            void            probe(equalizer &eqz, const actor *a, const readable<double> &C0);
            void            score(const readable<double> &C0, const equilibrium &);
            double          total(const eq_squad &);
            const eq_squad *champ(const eq_squad *, double &);
        };


    }

}

#endif

