//! \file

#ifndef YACK_CHEMICAL_XLIMIT_INCLUDED
#define YACK_CHEMICAL_XLIMIT_INCLUDED 1

#include "yack/chem/eqs/zlimit.hpp"

namespace yack
{
    namespace chemical
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

            //! search min of positive extents amongst actors
            /**
             \param A actors
             \param C global concentrations
             \param I index level to read C
             */
            bool search(const index_level       I,
                        const actors           &A,
                        const readable<double> &C);

            //! search limits within sub_level indices and top_level regularity
            bool search(const actors           &A,
                        const readable<double> &C,
                        const readable<bool>   &R);

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


