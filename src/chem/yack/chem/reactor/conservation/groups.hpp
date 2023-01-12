//! \file

#ifndef YACK_CHEMICAL_CL_GROUPS_INCLUDED
#define YACK_CHEMICAL_CL_GROUPS_INCLUDED 1

#include "yack/chem/reactor/conservation/group.hpp"

namespace yack
{
    namespace chemical
    {
        
        
        //______________________________________________________________________
        //
        //
        //! base class list for groups of independent c-laws
        //
        //______________________________________________________________________
        typedef cxx_list_of<cl_group> cl_groups_;


        //______________________________________________________________________
        //
        //
        //! groups of independent c-laws
        //
        //______________________________________________________________________
        class cl_groups : public object, public cl_groups_
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef auto_ptr<const cl_groups> ptr; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit cl_groups() throw(); //!< setup
            virtual ~cl_groups() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! two-steps placement of a new c-law
            void recruit(const claw &cl);


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(cl_groups);
        };

    }

}

#endif
