//! \file
#ifndef YACK_NWSORST_batcher10_INCLUDED
#define YACK_NWSORST_batcher10_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher10
    class batcher10 : public swaps {
      public:
        virtual ~batcher10() noexcept; //!< cleanup
        explicit batcher10() noexcept; //!< setup  
        static const char   sid[];        //!< batcher10
        static const size_t lhs[0x001f];  //!< lhs
        static const size_t rhs[0x001f];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher10);
    };
  }
}
#endif
