
//! \file

#ifndef YACK_AQUEOUS_XLIMITS_INCLUDED
#define YACK_AQUEOUS_XLIMITS_INCLUDED 1

#include "yack/aqueous/eqs/xlimit.hpp"
#include "yack/aqueous/eqs/components.hpp"

namespace yack
{
    namespace aqueous
    {
        //______________________________________________________________________
        //
        //
        //! availability state
        //
        //______________________________________________________________________
        enum availability
        {
            are_blocked, //!< missing at least one reac and at least one component
            are_running  //!< one side can move
        };

        //______________________________________________________________________
        //
        //
        //! different limitations
        //
        //______________________________________________________________________
        enum limitation
        {
            limited_by_none, //!< no limitation
            limited_by_reac, //!< limited by reac only
            limited_by_prod, //!< limited by prod only
            limited_by_both  //!< limited by both
        };


        //______________________________________________________________________
        //
        //
        //! find limiting extents and status
        //
        //______________________________________________________________________
        class xlimits : public sp_proxy
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit xlimits();            //!< setup
            virtual ~xlimits() noexcept;   //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! helper
            static const char * text(const limitation) noexcept;

            //! find extents and deduce flag
            limitation          operator()(const components       &,
                                           const readable<double> &);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            xlimit       reac; //!< reactant limits
            xlimit       prod; //!< product  limits
            availability flag; //!< flag

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(xlimits);
        };
    }

}

#endif

