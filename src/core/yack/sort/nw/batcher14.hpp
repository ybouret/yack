//! \file
#ifndef YACK_NWSORST_batcher14_INCLUDED
#define YACK_NWSORST_batcher14_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher14
    class batcher14 : public swaps {
      public:
        virtual ~batcher14() noexcept; //!< cleanup
        explicit batcher14() noexcept; //!< setup  
        static const char   sid[];        //!< batcher14
        static const size_t lhs[0x0035];  //!< lhs
        static const size_t rhs[0x0035];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher14);
    };
  }
}
#endif
