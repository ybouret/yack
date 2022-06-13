//! \file
#ifndef YACK_NWSORST_batcher28_INCLUDED
#define YACK_NWSORST_batcher28_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher28
    class batcher28 : public swaps {
      public:
        virtual ~batcher28() throw(); //!< cleanup
        explicit batcher28() throw(); //!< setup  
        static const char   sid[];        //!< batcher28
        static const size_t lhs[0x00a1];  //!< lhs
        static const size_t rhs[0x00a1];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher28);
    };
  }
}
#endif
