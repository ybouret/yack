//! \file
#ifndef YACK_NWSORST_batcher25_INCLUDED
#define YACK_NWSORST_batcher25_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher25
    class batcher25 : public swaps {
      public:
        virtual ~batcher25() throw(); //!< cleanup
        explicit batcher25() throw(); //!< setup  
        static const char   sid[];        //!< batcher25
        static const size_t lhs[0x008a];  //!< lhs
        static const size_t rhs[0x008a];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher25);
    };
  }
}
#endif
