
//! \file

#ifndef YACK_CHEMICAL_ACTIVE_INCLUDED
#define YACK_CHEMICAL_ACTIVE_INCLUDED 1

#include "yack/chem/sub-list.hpp"
#include "yack/chem/library.hpp"

namespace yack
{
    namespace chemical
    {

        typedef sub_list<species> alist; //!< alias
        typedef sub_node<species> anode; //!< alias

        //______________________________________________________________________
        //
        //
        //! sub list of active species
        //
        //______________________________________________________________________
        class active_list : public object, public alist
        {
        public:
            explicit active_list() throw();      //!< setup empty
            virtual ~active_list() throw();      //!< cleanup
            explicit active_list(const snode *); //!< from library
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(active_list);
        };



    }

}

#endif

