//! \file

#ifndef YACK_AQUEOUS_ZLIMIT_INCLUDED
#define YACK_AQUEOUS_ZLIMIT_INCLUDED 1

#include "yack/aqueous/eqs/actors.hpp"

namespace yack
{
    namespace aqueous
    {

        //______________________________________________________________________
        //
        //
        //! Zeroing Limit
        //
        //______________________________________________________________________
        class zlimit : public sp_repo
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit zlimit(const sp_proxy &) noexcept; //!< setup
            virtual ~zlimit()                 noexcept; //!< cleanup
            
            //! setup with first species/extent
            explicit zlimit(const sp_proxy &pp,
                            const double    xi,
                            const species  &sp);

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void     reset() noexcept;  //!< extent=0 and clear()
            YACK_PROTO_OSTREAM(zlimit); //!< display

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            double extent; //!<  extent zeroing species

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(zlimit);
        };

    



    }

}

#endif

