
//! \file

#ifndef YACK_CHEMICAL_GVECTOR_INCLUDED
#define YACK_CHEMICAL_GVECTOR_INCLUDED 1

#include "yack/chem/reactor/eq-repo.hpp"

namespace yack {

    namespace chemical {

        //______________________________________________________________________
        //
        //
        //! base class for a group of equilibria
        //
        //______________________________________________________________________
        typedef vector<eq_repo::ptr,memory::dyadic> gvector_;

        //______________________________________________________________________
        //
        //
        //! vector of shared list of equilibria
        //
        //______________________________________________________________________
        class gvector : public object, public gvector_
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef auto_ptr<const gvector> ptr; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit gvector();          //!< setup with first list
            virtual ~gvector() throw();  //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! ensure list of degree n exists, return content
            eq_repo & degree(const size_t n);


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(gvector);
            void grow();
        };
    }

}

#endif

