//! \file
#ifndef YACK_NWSORST_batcher8_INCLUDED
#define YACK_NWSORST_batcher8_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher8
    class batcher8 : public swaps {
      public:
        virtual ~batcher8() throw(); //!< cleanup
        explicit batcher8() throw(); //!< setup  
        static const char   sid[];        //!< batcher8
        static const size_t lhs[0x0013];  //!< lhs
        static const size_t rhs[0x0013];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher8);
    };
  }
}
#endif
