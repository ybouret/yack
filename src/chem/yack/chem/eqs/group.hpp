//! \file

#ifndef YACK_CHEM_EQS_GROUP_INCLUDED
#define YACK_CHEM_EQS_GROUP_INCLUDED 1


#include "yack/chem/equilibrium.hpp"
#include "yack/data/list/meta.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        // types to use a group of equilibria
        //
        //______________________________________________________________________
        typedef meta_list<const equilibrium> group_type; //!< alias
        typedef group_type::node_type        gnode;      //!< alias

        //______________________________________________________________________
        //
        //
        //!  group of equilibria
        //
        //______________________________________________________________________
        class group : public object, public group_type
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit group() throw(); //!< create empty
            virtual ~group() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            bool     connects(const equilibrium &rhs) const throw();          //!< check is a component of rhs is connected
            friend std::ostream & operator<<(std::ostream &, const group &); //!< display

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            group *next; //!< for groups
            group *prev; //!< for groups


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(group);
        };


    }

}

#endif

