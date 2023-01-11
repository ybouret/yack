
//! \file

#ifndef YACK_CHEMICAL_REACTOR_CUSTODIAN_INCLUDED
#define YACK_CHEMICAL_REACTOR_CUSTODIAN_INCLUDED 1

#include "yack/chem/reactor.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/data/dinky/solo-repo.hpp"
namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! custodian of conservation laws
        //
        //______________________________________________________________________
        class custodian : public large_object
        {
        public:

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit custodian(const reactor &); //!< setup
            virtual ~custodian() throw();        //!< cleanup


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void reset_all() throw(); //!< cleanup, mostly for debug

            //! correct all groups of laws
            /**
             - if true, then C0 is updated and dC is the total injected correction
             */
            bool corrected(writable<double> &C0);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const reactor        &cs; //!< persistent reactor
            const size_t          nc; //!< max number of claws/cluster
            cxx_array<double>     xs; //!< excess scores
            solo_repo<const claw> sr; //!< repository for algorithm
            matrix<double>        Cs; //!< storage of concentrations
            cxx_array<double>     dC; //!< corrected
            raddops               io; //!< internal




        private:
            YACK_DISABLE_COPY_AND_ASSIGN(custodian);
            bool corrected(writable<double>    &C0,
                           const cl_group      &cg);
        };

    }

}

#endif
