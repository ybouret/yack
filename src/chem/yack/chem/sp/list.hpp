//! \file

#ifndef YACK_CHEMICAL_SP_LIST_INCLUDED
#define YACK_CHEMICAL_SP_LIST_INCLUDED 1

#include "yack/chem/species.hpp"
#include "yack/data/list/meta.hpp"
#include "yack/associative/addrbook.hpp"

namespace yack {

    namespace chemical
    {

        typedef meta_list<const species> sp_list_;    //!< list of species, base class
        typedef sp_list_::node_type      sp_node;     //!< node within sp_list


        //! list of species, with sorting capabilities
        class sp_list : public sp_list_
        {
        public:
            explicit sp_list() throw(); //!< setup
            virtual ~sp_list() throw(); //!< cleanup
            void     sort()    throw(); //!< sort by increasing index
            void     load(const addrbook &tribe); //!< load and sort

            //! compare by increasing indices
            static int node_compare(const sp_node *, const sp_node *) throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(sp_list);
        };

    }

}

#endif

