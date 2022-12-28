//! \file

#ifndef YACK_CHEMICAL_EQ_REPO_INCLUDED
#define YACK_CHEMICAL_EQ_REPO_INCLUDED 1

#include "yack/chem/equilibrium.hpp"

namespace yack
{
    namespace chemical
    {
        typedef com_list<const equilibrium *> eq_repo_; //!< repositiroy
        typedef eq_repo_::pool_type           eq_pool;  //!< shared pool
        typedef eq_pool::pointer              eq_fund;  //!< shared fund
        typedef eq_repo_::node_type           eq_knot;  //!< node with sp_repo

        //! repository of equilibira with shared fund
        class eq_repo : public eq_repo_
        {
        public:
            explicit eq_repo(const eq_fund &) throw(); //!< setup
            virtual ~eq_repo() throw();                //!< cleanup

            //! display
            friend std::ostream & operator<<(std::ostream &, const eq_repo &);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(eq_repo);
        };

    }
}

#endif
