
//! \file

#ifndef YACK_AQUEOUS_FRONTIERS_INCLUDED
#define YACK_AQUEOUS_FRONTIERS_INCLUDED 1

#include "yack/aqueous/eqs/boundaries.hpp"
#include "yack/aqueous/eqs/xlimit.hpp"

namespace yack
{
    namespace aqueous
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

            //! classify all frontiers
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
