
//! \file

#ifndef YACK_AQUEOUS_BOUNDARIES_INCLUDED
#define YACK_AQUEOUS_BOUNDARIES_INCLUDED 1

#include "yack/aqueous/eqs/zlimit.hpp"
#include "yack/data/dinky/coop-list.hpp"

namespace yack
{
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

            //! insert with xi<0
            void     insert(const double   xi,
                            const species &sp);

            //! find correction subject to limitiation for size>0
            /**
             \param correction output
             \param limitation limiting reac for prod, prod for reac
             \return true if the correction rehabilitates
             */
            bool     find(zlimit       &correction,
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

