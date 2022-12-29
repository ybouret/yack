//! \file

#ifndef YACK_CHEMICAL_SP_REPO_INCLUDED
#define YACK_CHEMICAL_SP_REPO_INCLUDED 1

#include "yack/chem/species.hpp"
#include "yack/data/list/com.hpp"

namespace yack {

    namespace chemical
    {
        typedef com_list<const species *> sp_repo_; //!< repositiroy
        typedef sp_repo_::pool_type       sp_pool;  //!< shared pool
        typedef sp_pool::pointer          sp_fund;  //!< shared fund
        typedef sp_repo_::node_type       sp_knot;  //!< node with sp_repo

        //! repository of species with shared fund
        class sp_repo : public sp_repo_
        {
        public:
            explicit sp_repo(const sp_fund &) throw(); //!< setup
            virtual ~sp_repo() throw();                //!< cleanup

            //! display
            friend std::ostream & operator<<(std::ostream &, const sp_repo &);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(sp_repo);
        };

    }

}

#endif

