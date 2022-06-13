//! \file
#ifndef YACK_NWSORST_batcher2_INCLUDED
#define YACK_NWSORST_batcher2_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher2
    class batcher2 : public swaps {
      public:
        virtual ~batcher2() throw(); //!< cleanup
        explicit batcher2() throw(); //!< setup  
        static const char   sid[];        //!< batcher2
        static const size_t lhs[0x0001];  //!< lhs
        static const size_t rhs[0x0001];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher2);
    };
  }
}
#endif
