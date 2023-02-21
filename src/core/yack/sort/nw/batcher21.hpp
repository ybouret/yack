//! \file
#ifndef YACK_NWSORST_batcher21_INCLUDED
#define YACK_NWSORST_batcher21_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher21
    class batcher21 : public swaps {
      public:
        virtual ~batcher21() noexcept; //!< cleanup
        explicit batcher21() noexcept; //!< setup  
        static const char   sid[];        //!< batcher21
        static const size_t lhs[0x006b];  //!< lhs
        static const size_t rhs[0x006b];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher21);
    };
  }
}
#endif
