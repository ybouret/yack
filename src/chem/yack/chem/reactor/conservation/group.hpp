
//! \file

#ifndef YACK_CHEMICAL_CL_GROUP_INCLUDED
#define YACK_CHEMICAL_CL_GROUP_INCLUDED 1

#include "yack/chem/reactor/conservation/laws.hpp"
#include "yack/data/dinky/core-repo.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! base class for a group c-laws
        //
        //______________________________________________________________________
        typedef core_repo<const claw> cl_repo;
        typedef cl_repo::node_type    cl_node; //!< alias


        //______________________________________________________________________
        //
        //
        //! group of interdependent conservation laws
        //
        //______________________________________________________________________
        class clot : public object, public cl_repo
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit clot(const claw &); //!< create with first conservation law
            virtual ~clot()     throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            bool     attached_to(const claw &) const throw(); //!< test attached to c-law
            bool     attached_to(const clot &) const throw(); //!< test attached to another group

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            clot *next; //!< for list
            clot *prev; //!< for list

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(clot);
        };
    }

}

#endif
