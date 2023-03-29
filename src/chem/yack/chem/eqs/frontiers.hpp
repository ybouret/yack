
//! \file

#ifndef YACK_CHEMICAL_FRONTIERS_INCLUDED
#define YACK_CHEMICAL_FRONTIERS_INCLUDED 1

#include "yack/chem/eqs/boundaries.hpp"
#include "yack/chem/eqs/xlimit.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! frontiers for a set of actors
        //
        //______________________________________________________________________
        class frontiers
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit frontiers(const zl_proxy &, const sp_proxy &) noexcept; //!< setup
            virtual ~frontiers()                                   noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! erase everything
            void        initialize() noexcept;

            //! classify all actors w.r.t their concentration and regularity
            /**
             \param A actors
             \param C concentrations
             \param R global regular flags
             \return true if no out of range
             */
            bool        honored_by(const actors           &A,
                                   const readable<double> &C,
                                   const readable<bool>   &R);


            //! display
            YACK_PROTO_OSTREAM(frontiers);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            xlimit      lim; //!<  limiting extent from c>0
            boundaries  oor; //!<  out of range extent from c<0

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(frontiers);
        };
    }

}


#endif
