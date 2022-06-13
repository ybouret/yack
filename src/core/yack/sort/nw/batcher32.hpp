//! \file
#ifndef YACK_NWSORST_batcher32_INCLUDED
#define YACK_NWSORST_batcher32_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher32
    class batcher32 : public swaps {
      public:
        virtual ~batcher32() throw(); //!< cleanup
        explicit batcher32() throw(); //!< setup  
        static const char   sid[];        //!< batcher32
        static const size_t lhs[0x00bf];  //!< lhs
        static const size_t rhs[0x00bf];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher32);
    };
  }
}
#endif
