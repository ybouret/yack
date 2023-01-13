//! \file

#ifndef YACK_CHEMICAL_EQ_REPO_INCLUDED
#define YACK_CHEMICAL_EQ_REPO_INCLUDED 1

#include "yack/chem/equilibria.hpp"
#include "yack/data/dinky/core-repo.hpp"
#include "yack/chem/sub-list.hpp"

namespace yack {

    namespace chemical {

        //______________________________________________________________________
        //
        //
        // base classes
        //
        //______________________________________________________________________
        typedef core_repo<const equilibrium>   eq_repo_; //!< alias
        typedef eq_repo_::node_type            eq_node;  //!< alias

        //______________________________________________________________________
        //
        //
        //! counted repo
        //
        //______________________________________________________________________
        class eq_repo : public object, public counted, public eq_repo_
        {
        public:
            //__________________________________________________________________
            //
            // types
            //__________________________________________________________________
            typedef  arc_ptr<eq_repo> ptr; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit eq_repo() throw(); //!< setup
            virtual ~eq_repo() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! display
            friend std::ostream & operator<<(std::ostream &, const eq_repo &);


            //! viz
            void viz(ios::ostream            &fp,
                     const sub_list<species> &act,
                     const readable<bool>    &flg,
                     const unsigned           igv) const;



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(eq_repo);
        };

    }

}

#endif

