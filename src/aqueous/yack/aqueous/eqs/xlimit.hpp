
//! \file

#ifndef YACK_AQUEOUS_XLIMIT_INCLUDED
#define YACK_AQUEOUS_XLIMIT_INCLUDED 1

#include "yack/aqueous/eqs/zlimit.hpp"

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
        class xlimit : public zlimit
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

            //! search min of positive extents
            /**
             \param A actors
             \param C global concentrations
             */
            bool search(const actors           &A,
                        const readable<double> &C);  

            //! insert new value
            /**
             \param xi xi>=0
             \param sp limiting species
             */
            void insert(const double   xi,
                        const species &sp);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(xlimit);
            
        };

    }

}

#endif


