//! \file

#ifndef YACK_AQUEOUS_CONSERVATION_INCLUDED
#define YACK_AQUEOUS_CONSERVATION_INCLUDED 1

#include "yack/aqueous/eqs/actors.hpp"
//#include "yack/data/dinky/core-repo.hpp"

namespace yack
{
    namespace aqueous
    {
        //______________________________________________________________________
        //
        //
        //! conservation law
        //
        //______________________________________________________________________
        class conservation : public object, public actors_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit conservation() noexcept; //!< setup
            virtual ~conservation() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            YACK_PROTO_OSTREAM(conservation); //!< display

            bool overlaps(const conservation &) const noexcept;

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            conservation *next; //!< for list
            conservation *prev; //!< for list
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(conservation);
        };

        //______________________________________________________________________
        //
        //
        //! conservation laws
        //
        //______________________________________________________________________
        typedef cxx_list_of<conservation> conservations;


        //______________________________________________________________________
        //
        //
        //! base class for conserved
        //
        //______________________________________________________________________
        typedef core_repo<const conservation> conserved_;

        //______________________________________________________________________
        //
        //
        //! group of dependent conservation laws
        //
        //______________________________________________________________________
        class conserved : public object, public conserved_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit conserved(const conservation &first);
            virtual ~conserved() noexcept;

            bool accepts(const conservation &) const noexcept;

            //__________________________________________________________________
            //
            //  members
            //__________________________________________________________________
            conserved *next; //!< for list
            conserved *prev; //!< for list

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(conserved);
        };

        typedef cxx_list_of<conserved> conserved_group;

    }
}

#endif

