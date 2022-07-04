
//! \file

#ifndef YACK_CHEM_MIX_GROUPS_INCLUDED
#define YACK_CHEM_MIX_GROUPS_INCLUDED 1

#include "yack/chem/mix/group.hpp"

namespace yack
{
    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! groups sharing same properties
        //
        //______________________________________________________________________
        class groups : public group::list
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~groups() throw(); //!< cleanup
            explicit groups() throw(); //!< setup

            //! create groups of ATTACHED equilibria, and set info accordingly
            explicit groups(const equilibria &eqs);

            //! create groups of detached equilibria
            /**
             reply to : what are the equilibrium within an attached group
             that can be tested simultaneously ?
             */
            explicit groups(const group  &attached, const matrix<bool> &detached);

            //! assemble groups from partitions of groups
            void weave(const readable<groups> &part);

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void sort(); //!< full sort groups

            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________
            friend std::ostream & operator<<(std::ostream &os, const groups &); //!< display

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(groups);
        };

    }


}

#endif

