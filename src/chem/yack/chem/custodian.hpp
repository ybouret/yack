
//! \file

#ifndef YACK_CHEMICAL_CUSTODIAN_INCLUDED
#define YACK_CHEMICAL_CUSTODIAN_INCLUDED 1

#include "yack/chem/nexus.hpp"


namespace yack
{
    
    namespace chemical
    {
        //! will apply conservation laws
        class custodian
        {
        public:
            explicit custodian(const nexus &);
            virtual ~custodian() throw();



            void operator()(writable<double> &C0);
            

            const nexus &heart;  //!< nexus
            const size_t count;  //!< heart.Nq
            tableau      score;  //!< [count]
            rmatrix      state;  //!< [count:M]
            claw_repo    alive;  //!< [0..count]
            raddops      xadd;   //!<

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(custodian);
            void abide(writable<double> &C0,
                       const claw_team  &claws);

            void abide(writable<double>  &C0,
                       const cluster     &cc);
        };
        
    }
}

#endif

