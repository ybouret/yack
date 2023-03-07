//! \file

#ifndef YACK_CHEMICAL_ACTIVE_INCLUDED
#define YACK_CHEMICAL_ACTIVE_INCLUDED 1

#include "yack/chem/sub-list.hpp"
#include "yack/chem/library.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{
    namespace chemical
    {

        typedef sub_list<species> alist_; //!< alias
        typedef sub_node<species> anode;  //!< alias

        //______________________________________________________________________
        //
        //
        //! sub list of active species for 
        //
        //______________________________________________________________________
        class alist : public alist_
        {
        public:
            typedef auto_ptr<const alist> ptr; //!< alias
            explicit alist() noexcept;         //!< setup empty
            virtual ~alist() noexcept;         //!< cleanup
            explicit alist(const snode *);     //!< active from library


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(alist);
        };



    }

}

#endif

