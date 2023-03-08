
//! \file

#ifndef YACK_AQUEOUS_EQUILIBRIA_INCLUDED
#define YACK_AQUEOUS_EQUILIBRIA_INCLUDED 1

#include "yack/aqueous/equilibrium.hpp"
#include "yack/aqueous/gathering.hpp"

namespace yack
{
    namespace aqueous
    {
        //______________________________________________________________________
        //
        //
        // types for equilibria
        //
        //______________________________________________________________________
        typedef suffix_set<string,equilibrium::ptr> eqs_db; //!< database
        typedef eqs_db::knot_type                   enode;  //!< alias

        //______________________________________________________________________
        //
        //
        //! equilibrium = components + entity + constant(time)
        //
        //______________________________________________________________________
        class equilibria : public gathering
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            static const char * const clid; //!< "equilibria"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit equilibria() noexcept; //!< setup emptys
            virtual ~equilibria() noexcept; //!< cleanup
            equilibria(const equilibria &); //!< hard cloning

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const eqs_db::tree_type & operator->() const noexcept; //!< drill-down

            size_t        next_indx() const noexcept; //!< size+1
            equilibrium & operator()( equilibrium *eq );


        private:
            YACK_DISABLE_ASSIGN(equilibria);
            eqs_db edb;
        };

    }

}

#endif
