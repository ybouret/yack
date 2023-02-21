//! \file
#ifndef YACK_NWSORST_hibbard25_INCLUDED
#define YACK_NWSORST_hibbard25_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard25
    class hibbard25 : public swaps {
      public:
        virtual ~hibbard25() noexcept; //!< cleanup
        explicit hibbard25() noexcept; //!< setup  
        static const char   sid[];        //!< hibbard25
        static const size_t lhs[0x009a];  //!< lhs
        static const size_t rhs[0x009a];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard25);
    };
  }
}
#endif
