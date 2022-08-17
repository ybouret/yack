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
            group(const group &);     //!< copy

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            bool     accepts(const equilibrium &rhs) const throw();          //!< check is a component of rhs is connected
            friend std::ostream & operator<<(std::ostream &, const group &); //!< display
            void   sort(); //!< by equilibrium index

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            group *next; //!< for groups
            group *prev; //!< for groups


        private:
            YACK_DISABLE_ASSIGN(group);
        };


    }

}

#endif

