

#ifndef YACK_CHEM_EQS_GROUPS_INCLUDED
#define YACK_CHEM_EQS_GROUPS_INCLUDED 1


#include "yack/chem/eqs/group.hpp"

namespace yack
{
    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //!  groups of group of equilibria
        //
        //______________________________________________________________________
        class groups :  public cxx_list_of<group>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit groups() throw(); //!< setup empty
            virtual ~groups() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            friend std::ostream & operator<<(std::ostream &, const groups &); //!< display

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(groups);
        };


    }

}

#endif

