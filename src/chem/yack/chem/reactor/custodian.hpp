
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

        class custodian : public large_object
        {
        public:

            explicit custodian(const reactor &);
            virtual ~custodian() throw();

            const reactor        &cs; //!< persistent reactor
            const size_t          nc; //!< max number of claws/cluster
            cxx_array<double>     xs; //!< excess scores
            solo_repo<const claw> sr; //!< repository for algorithm
            matrix<double>        Cs; //!< storage of concentrations
            cxx_array<double>     dC; //!< corrected
            raddops               io; //!< internal

            void reset_all() throw();
            bool corrected(writable<double> &C0);



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(custodian);
            bool corrected(writable<double>    &C0,
                           const cl_group      &cg);
        };

    }

}

#endif
