//! \file

#ifndef YACK_SORT_NWSWP_INCLUDED
#define YACK_SORT_NWSWP_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace nwsrt
    {
        
        class swaps
        {
        public:
            virtual ~swaps() throw();
            
            const char * const   name; //!< uuid
            const size_t         size; //!< inputs
            const size_t         work; //!< comparisons
            const size_t * const ltab; //!< left  index tableau
            const size_t * const rtab; //!< right index tableau

        protected:
            explicit swaps(const char  *sid,
                           const size_t dim,
                           const size_t num,
                           const size_t *lhs,
                           const size_t *rhs) throw();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(swaps);
            
        };
        
        
        
    };
}

#endif

