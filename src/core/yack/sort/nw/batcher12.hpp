//! \file
#ifndef YACK_NWSORST_batcher12_INCLUDED
#define YACK_NWSORST_batcher12_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher12
    class batcher12 : public swaps {
      public:
        virtual ~batcher12() noexcept; //!< cleanup
        explicit batcher12() noexcept; //!< setup  
        static const char   sid[];        //!< batcher12
        static const size_t lhs[0x0029];  //!< lhs
        static const size_t rhs[0x0029];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher12);
    };
  }
}
#endif
