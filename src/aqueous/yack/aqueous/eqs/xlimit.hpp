
//! \file

#ifndef YACK_AQUEOUS_XLIMIT_INCLUDED
#define YACK_AQUEOUS_XLIMIT_INCLUDED 1

#include "yack/aqueous/eqs/actors.hpp"

namespace yack
{
    namespace aqueous
    {

        //______________________________________________________________________
        //
        //
        //! eXtent Limit
        //
        //______________________________________________________________________
        class xlimit : public sp_repo
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit xlimit(const sp_proxy &) noexcept; //!< setup
            virtual ~xlimit()                 noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void     initialize() noexcept; //!< clear and xi=0
            bool     get_extent(const actors           &A,
                                const readable<double> &C); //!< find min of positive extents
            YACK_PROTO_OSTREAM(xlimit);     //!< display

            //__________________________________________________________________
            //
            // member
            //__________________________________________________________________
            double         xi; //!< maximal possible extent
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(xlimit);
        };

    }

}

#endif


