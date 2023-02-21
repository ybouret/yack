//! \file
#ifndef YACK_NWSORST_batcher7_INCLUDED
#define YACK_NWSORST_batcher7_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher7
    class batcher7 : public swaps {
      public:
        virtual ~batcher7() noexcept; //!< cleanup
        explicit batcher7() noexcept; //!< setup  
        static const char   sid[];        //!< batcher7
        static const size_t lhs[0x0010];  //!< lhs
        static const size_t rhs[0x0010];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher7);
    };
  }
}
#endif
