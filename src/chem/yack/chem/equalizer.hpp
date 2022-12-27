
//! \file

#ifndef YACK_CHEMICAL_EQUALIZER_INCLUDED
#define YACK_CHEMICAL_EQUALIZER_INCLUDED 1

#include "yack/chem/limiting.hpp"
#include "yack/chem/boundary.hpp"

namespace yack
{

    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! hold limiting and boundaries for reactants/products
        //
        //______________________________________________________________________
        class equalizer
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            equalizer(const size_t m, const sp_fund &io); //!< setup memory
            ~equalizer() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void destroy() throw(); //!< cleanup all


            //! syntax helper: neg.analyze(fade,other.lim)
            void look_up(frontier &fade, const equalizer &other) const;

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            limiting   lim; //!< limiting extent
            boundaries neg; //!< negative extents

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(equalizer);
        };

    }

}

#endif

