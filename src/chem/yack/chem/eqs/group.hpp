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
            bool     accepts(const equilibrium &) const throw();  //!< check is a component of equilibrium is connected
            void     sort();                                      //!< by increasing equilibrium index
            bool     includes(const equilibrium &) const throw(); //!< check equilibrium is included in this group
            bool     excludes(const equilibrium &) const throw(); //!< check equilibrium is excluded of this group
            bool     contains(const group       &) const throw(); //!< check all equilibria are within this group
            bool     is_ortho() const throw(); //!< all are disconnected

            friend std::ostream & operator<<(std::ostream &, const group &); //!< display

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

