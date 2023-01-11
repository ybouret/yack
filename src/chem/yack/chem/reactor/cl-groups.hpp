//! \file

#ifndef YACK_CHEMICAL_CL_GROUPS_INCLUDED
#define YACK_CHEMICAL_CL_GROUPS_INCLUDED 1

#include "yack/chem/reactor/claws.hpp"
#include "yack/data/dinky/core-repo.hpp"
#include "yack/container/matrix.hpp"

namespace yack
{
    namespace chemical
    {
        typedef core_repo<const claw> cl_repo;

        //! group of interdpendent conservation laws
        class cl_group : public object, public cl_repo
        {
        public:
            explicit cl_group(const claw &first);
            virtual ~cl_group() throw();
            bool     attached_to(const claw     &) const throw();
            bool     attached_to(const cl_group &) const throw();

            cl_group *next;
            cl_group *prev;
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(cl_group);
        };


        typedef cxx_list_of<cl_group> cl_groups_;

        class cl_groups : public object, public cl_groups_
        {
        public:
            typedef auto_ptr<const cl_groups> ptr;
            explicit cl_groups() throw();
            virtual ~cl_groups() throw();

            void recruit(const claw &cl);


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(cl_groups);
        };

    }

}

#endif
