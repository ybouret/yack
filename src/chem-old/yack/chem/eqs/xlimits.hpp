
//! \file

#ifndef YACK_CHEMICAL_XLIMITS_INCLUDED
#define YACK_CHEMICAL_XLIMITS_INCLUDED 1

#include "yack/chem/eqs/xlimit.hpp"
#include "yack/chem/eqs/components.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! availability state
        //
        //______________________________________________________________________
        enum availability
        {
            is_blocked, //!< missing at least one reac and at least one component
            is_running  //!< one side can move
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


            static const char * limit_text(const limitation)   noexcept;  //!< helper
            static const char * avail_text(const availability) noexcept;  //!< helper


            //! find extents and deduce flag
            limitation          operator()(const index_level       I,
                                           const components       &E,
                                           const readable<double> &C);

            //! find extents for sub_level using top_level regularity
            limitation          operator()(const components       &E,
                                           const readable<double> &C,
                                           const readable<bool>   &R);
            
            YACK_PROTO_OSTREAM(xlimits); //!< display
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            xlimit       reac; //!< reactant limits
            xlimit       prod; //!< product  limits
            limitation   type; //!< computed limitation
            availability flag; //!< computed availability

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(xlimits);
        };
    }

}

#endif
