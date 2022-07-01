
//! \file

#ifndef YACK_CHEM_MIX_ACTIVE_INCLUDED
#define YACK_CHEM_MIX_ACTIVE_INCLUDED 1

#include "yack/chem/library.hpp"
#include "yack/data/list/meta.hpp"

namespace yack
{
    namespace chemical
    {

        typedef meta_list<const species> alist_; //!< alias
        typedef alist_::node_type        anode;  //!< alias

        //______________________________________________________________________
        //
        //
        //! list of active species
        //
        //______________________________________________________________________
        class alist : public alist_
        {
        public:
            virtual ~alist() throw();         //!< cleanup
            explicit alist(const library &);  //!< build from positive ranked species

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(alist);
        };
    }

}

#endif
