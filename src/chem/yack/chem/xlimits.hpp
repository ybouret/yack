
//! \file

#ifndef YACK_CHEMICAL_XLIMITS_INCLUDED
#define YACK_CHEMICAL_XLIMITS_INCLUDED 1

#include "yack/chem/xlimit.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //! kind of limitation per components/equilibria
        //______________________________________________________________________
        enum limitation
        {
            limited_by_none, //!< no limitation
            limited_by_reac, //!< limited by a positive extent
            limited_by_prod, //!< limited by a negatice extent
            limited_by_both  //!< limited by both extents
        };
        
        //______________________________________________________________________
        //
        //! aggregate and type of xlimits
        //______________________________________________________________________
        class xlimits
        {
        public:
            
            xlimits(const xlimit *rx,
                    const xlimit *px,
                    const size_t  wm) throw(); //!< setup from individual limits

            //! display
            friend std::ostream & operator<<(std::ostream &, const xlimits &);

            //! check if extent is acceptable : no negative of zero concentration
            bool acceptable(const double xx) const throw();

            const xlimit * const reac; //!< NULL or limiting reactant
            const xlimit * const prod; //!< NULL or limitins product
            const limitation     type; //!< computed
            const size_t         wmax; //!< helper to format




            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(xlimits);
            ~xlimits() throw();
        };
    }
    
}

#endif

