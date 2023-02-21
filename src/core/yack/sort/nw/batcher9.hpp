//! \file
#ifndef YACK_NWSORST_batcher9_INCLUDED
#define YACK_NWSORST_batcher9_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher9
    class batcher9 : public swaps {
      public:
        virtual ~batcher9() noexcept; //!< cleanup
        explicit batcher9() noexcept; //!< setup  
        static const char   sid[];        //!< batcher9
        static const size_t lhs[0x001a];  //!< lhs
        static const size_t rhs[0x001a];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher9);
    };
  }
}
#endif
