//! \file
#ifndef YACK_NWSORST_batcher22_INCLUDED
#define YACK_NWSORST_batcher22_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher22
    class batcher22 : public swaps {
      public:
        virtual ~batcher22() throw(); //!< cleanup
        explicit batcher22() throw(); //!< setup  
        static const char   sid[];        //!< batcher22
        static const size_t lhs[0x0072];  //!< lhs
        static const size_t rhs[0x0072];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher22);
    };
  }
}
#endif
