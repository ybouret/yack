//! \file
#ifndef YACK_NWSORST_hibbard22_INCLUDED
#define YACK_NWSORST_hibbard22_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! hibbard22
    class hibbard22 : public swaps {
      public:
        virtual ~hibbard22() noexcept; //!< cleanup
        explicit hibbard22() noexcept; //!< setup  
        static const char   sid[];        //!< hibbard22
        static const size_t lhs[0x007d];  //!< lhs
        static const size_t rhs[0x007d];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(hibbard22);
    };
  }
}
#endif
