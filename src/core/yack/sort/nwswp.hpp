//! \file

#ifndef YACK_SORT_NWSWP_INCLUDED
#define YACK_SORT_NWSWP_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace nwsrt
    {
        
        //! base class to collect precomputed swaps
        class swaps
        {
        public:
            virtual ~swaps() noexcept;  //!< cleanup
            
            const char * const   name; //!< uuid
            const size_t         size; //!< inputs
            const size_t         work; //!< comparisons
            const size_t * const ltab; //!< left  index tableau
            const size_t * const rtab; //!< right index tableau

            //! display swap indices (-offset)
            void display(const size_t offset=0) const;

        protected:
            //! setup full metrics
            explicit swaps(const char  *sid,
                           const size_t dim,
                           const size_t num,
                           const size_t *lhs,
                           const size_t *rhs) noexcept;
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(swaps);
            
        };
        
        
        
    };
}

#endif

