
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
        //! group of interdpendent conservation laws
        //
        //______________________________________________________________________
        class cl_group : public object, public cl_repo
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit cl_group(const claw &); //!< create with first c-law
            virtual ~cl_group()     throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            bool     attached_to(const claw     &) const throw(); //!< test attached to c-law
            bool     attached_to(const cl_group &) const throw(); //!< test attached to another group

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            cl_group *next; //!< for list
            cl_group *prev; //!< for list
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(cl_group);
        };
    }

}

#endif
