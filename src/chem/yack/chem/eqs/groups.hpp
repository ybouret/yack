

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
            groups(const groups &);    //!< copy
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            friend std::ostream & operator<<(std::ostream &, const groups &); //!< display

            void sort(); //!< sort all groups then per group size then per lexicographic order

        private:
            YACK_DISABLE_ASSIGN(groups);
        };


    }

}

#endif

