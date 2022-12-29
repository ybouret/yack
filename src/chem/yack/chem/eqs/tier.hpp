//! \file

#ifndef YACK_CHEMICAL_EQ_TIER_INCLUDED
#define YACK_CHEMICAL_EQ_TIER_INCLUDED 1

#include "yack/chem/eqs/team.hpp"
#include "yack/chem/sp/list.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{
    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! for classification of equilibria
        //
        //______________________________________________________________________
        class eq_tier : public object
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef auto_ptr<eq_tier> pointer; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit eq_tier() throw(); //!< setup empty
            virtual ~eq_tier() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! extract all species into target
            static void compose(sp_list &target, const eq_team &source);

            //! compose unbridled and regulated
            void        compile();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const eq_team     roaming;   //!< roaming equilibiria
            const eq_team     bounded;   //!< bounded equilibria
            const sp_list     unbridled; //!< unbridled species from roaming
            const sp_list     regulated; //!< regulated species from bounded

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(eq_tier);
        };

    }

}

#endif

