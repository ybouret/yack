//! \file

#ifndef YACK_CHEMICAL_REACTOR_INVOLVED_INCLUDED
#define YACK_CHEMICAL_REACTOR_INVOLVED_INCLUDED 1

#include "yack/chem/equilibrium.hpp"
#include "yack/data/list/meta.hpp"
#include "yack/sequence/vector.hpp"

namespace yack
{

    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        // types for list of involved equilibria
        //
        //______________________________________________________________________
        typedef meta_list<const equilibrium>       islot_; //!< alias for list of involved equilibri(um|a)
        typedef islot_::node_type                  inode; //!< alias for node of involved equilirium
        //______________________________________________________________________
        //
        //! derived meta list of equilibria
        //______________________________________________________________________
        class islot : public islot_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit islot() throw(); //!< setup
            virtual ~islot() throw(); //!< cleanup
            islot(const islot &);     //!< copy

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! display as list
            friend std::ostream & operator<<(std::ostream &, const islot &);

        private:
            YACK_DISABLE_ASSIGN(islot);
        };

        typedef vector<const islot,memory::dyadic> slots; //!< alias

    }

}
#endif

