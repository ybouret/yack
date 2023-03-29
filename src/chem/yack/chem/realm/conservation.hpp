//! \file

#ifndef YACK_CHEMICAL_CONSERVATION_INCLUDED
#define YACK_CHEMICAL_CONSERVATION_INCLUDED 1

#include "yack/chem/eqs/actors.hpp"
#include "yack/cameo/add.hpp"

namespace yack
{
    namespace chemical
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
            YACK_PROTO_OSTREAM(conservation);                     //!< display
            bool   overlaps(const conservation &) const noexcept; //!< testing species in common
            void   finalize() noexcept;                           //!< update nrm2
            double excess(const readable<double> &C, cameo::add<double> &) const; //!< <*this|C>
            //void   deltaC(writable<double> &dC, const double) const noexcept;

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const double  nrm2; //!< sum(nu^2)
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


      

    }
}

#endif

