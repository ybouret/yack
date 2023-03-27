
//! \file

#ifndef YACK_AQUEOUS_BOUNDARIES_INCLUDED
#define YACK_AQUEOUS_BOUNDARIES_INCLUDED 1

#include "yack/aqueous/eqs/zlimit.hpp"
#include "yack/data/dinky/coop-list.hpp"

namespace yack
{
    class xmlog;
    
    namespace aqueous
    {
        //______________________________________________________________________
        //
        //
        // types for boundaries
        //
        //______________________________________________________________________
        typedef coop_list<zlimit>       zlimits;  //!< base class for boundaries
        typedef zlimits::node_type      zl_node;  //!< alias
        typedef zlimits::proxy_type     zl_proxy; //!< alias
        typedef zlimits::zpool_type     zl_zpool; //!< alis

        enum shift_status
        {
            shift_success,
            shift_partial,
            shift_blocked
        };

        //______________________________________________________________________
        //
        //
        //! boundaries for negative concentrations
        //
        //______________________________________________________________________
        class boundaries : public zlimits 
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! construct with cache for zlimits and each zlimit
            explicit boundaries(const zl_proxy &,
                                const sp_proxy &) noexcept;
            virtual ~boundaries() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! insert with xi>0, comming from a negative concentration
            void     insert(double         xi,
                            const species &sp);

            //! find correction subject to limitiation for this->size>0
            /**
             \param xml        for output
             \param correction result
             \param limitation limiting reac for prod, prod for reac
             \return true if the correction rehabilitates
             */
            shift_status     try_shift(const xmlog  &xml,
                                       zlimit       &correction,
                                       const zlimit &limitation) const;



        private:
            YACK_DISABLE_COPY_AND_ASSIGN(boundaries);
            const sp_proxy spore; //!< used for each zlimit
            void insert1(const double xi, const species &sp);
            bool check() const noexcept;
            bool check_valid() const noexcept;
        };

    }
}

#endif

