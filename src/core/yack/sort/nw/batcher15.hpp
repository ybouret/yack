//! \file
#ifndef YACK_NWSORST_batcher15_INCLUDED
#define YACK_NWSORST_batcher15_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher15
    class batcher15 : public swaps {
      public:
        virtual ~batcher15() throw(); //!< cleanup
        explicit batcher15() throw(); //!< setup  
        static const char   sid[];        //!< batcher15
        static const size_t lhs[0x003b];  //!< lhs
        static const size_t rhs[0x003b];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher15);
    };
  }
}
#endif
