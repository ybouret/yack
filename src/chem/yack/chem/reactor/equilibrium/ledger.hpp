
//! \file

#ifndef YACK_CHEMICAL_GVECTOR_INCLUDED
#define YACK_CHEMICAL_GVECTOR_INCLUDED 1

#include "yack/chem/reactor/equilibrium/repo.hpp"

namespace yack {

    namespace chemical {

        //______________________________________________________________________
        //
        //
        //! base class for a group of equilibria
        //
        //______________________________________________________________________
        typedef vector<eq_repo::ptr,memory::dyadic> ledger_;

        //______________________________________________________________________
        //
        //
        //! ledger of shared list of equilibria
        //
        //______________________________________________________________________
        class ledger : public object, public ledger_
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef auto_ptr<const ledger> ptr; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit ledger();          //!< setup with first list
            virtual ~ledger() noexcept;  //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! ensure list of degree n exists, return content
            eq_repo & degree(const size_t n);


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ledger);
            void grow();
        };
    }

}

#endif

