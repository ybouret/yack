//! \file
#ifndef YACK_NWSORST_batcher27_INCLUDED
#define YACK_NWSORST_batcher27_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher27
    class batcher27 : public swaps {
      public:
        virtual ~batcher27() noexcept; //!< cleanup
        explicit batcher27() noexcept; //!< setup  
        static const char   sid[];        //!< batcher27
        static const size_t lhs[0x009b];  //!< lhs
        static const size_t rhs[0x009b];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher27);
    };
  }
}
#endif
