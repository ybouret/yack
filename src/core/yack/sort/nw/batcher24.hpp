//! \file
#ifndef YACK_NWSORST_batcher24_INCLUDED
#define YACK_NWSORST_batcher24_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher24
    class batcher24 : public swaps {
      public:
        virtual ~batcher24() noexcept; //!< cleanup
        explicit batcher24() noexcept; //!< setup  
        static const char   sid[];        //!< batcher24
        static const size_t lhs[0x007f];  //!< lhs
        static const size_t rhs[0x007f];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher24);
    };
  }
}
#endif
