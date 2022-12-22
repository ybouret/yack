
//! \file

#ifndef YACK_CHEMICAL_CUSTODIAN_INCLUDED
#define YACK_CHEMICAL_CUSTODIAN_INCLUDED 1

#include "yack/chem/nexus.hpp"
#include "yack/type/authority.hpp"


namespace yack
{
    
    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! will apply conservation laws
        //
        //______________________________________________________________________
        class custodian : public authority<const nexus>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit custodian(const nexus &, const xmlog &); //!< setup
            virtual ~custodian() throw();                     //!< cleanup


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! inject minimal concentration to fullfill all laws
            /**
             C0 is updated and a track of injectect concentrations
             is kept in 'inset'
             */
            void operator()(writable<double> &C0);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t rules;  //!< nexus.Nq, total number of laws
            const size_t items;  //!< nexus.M,  total number of species
            tableau      score;  //!< [count]   comparative scores
            tableau      inset;  //!< [M]       cumulative correction
            rmatrix      state;  //!< [count:M] corrected configurations
            claw_repo    alive;  //!< [0..count]
            raddops      xadd;   //!< for internal ops
            const xmlog &xml;    //!< for verbosity
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(custodian);
            void abide(writable<double> &C0,
                       const claw_team  &claws);

            void abide(writable<double>  &C0,
                       const cluster     &cc);

            bool       initialize_with(const readable<double> &, const claw_team &);
            claw_node *select_next_law() throw();
            bool       need_to_process(const readable<double> &);
        };
        
    }
}

#endif

