
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

            //! create groups of attached equilibria, and set info accordingly
            explicit groups(const equilibria &eqs);

            //! create groups of detached equilibria
            explicit groups(const group  &attached);

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

