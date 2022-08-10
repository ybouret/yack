
//! \file

#ifndef YACK_CHEMICAL_XLIMITS_INCLUDED
#define YACK_CHEMICAL_XLIMITS_INCLUDED 1

#include "yack/chem/xlimit.hpp"

namespace yack
{
    namespace chemical
    {
        enum limitation
        {
            limited_by_none,
            limited_by_reac,
            limited_by_prod,
            limited_by_both
        };
        
        class xlimits
        {
        public:
            xlimits(const xlimit *rx,
                    const xlimit *px,
                    const size_t  wm) throw();
            
            const xlimit * const reac;
            const xlimit * const prod;
            const limitation     type;
            const size_t         wmax;
            
            friend std::ostream & operator<<(std::ostream &, const xlimits &);
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(xlimits);
            ~xlimits() throw();
        };
    }
    
}

#endif

