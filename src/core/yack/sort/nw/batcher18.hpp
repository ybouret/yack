//! \file
#ifndef YACK_NWSORST_batcher18_INCLUDED
#define YACK_NWSORST_batcher18_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher18
    class batcher18 : public swaps {
      public:
        virtual ~batcher18() noexcept; //!< cleanup
        explicit batcher18() noexcept; //!< setup  
        static const char   sid[];        //!< batcher18
        static const size_t lhs[0x0052];  //!< lhs
        static const size_t rhs[0x0052];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher18);
    };
  }
}
#endif
