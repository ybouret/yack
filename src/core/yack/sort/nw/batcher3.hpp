//! \file
#ifndef YACK_NWSORST_batcher3_INCLUDED
#define YACK_NWSORST_batcher3_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher3
    class batcher3 : public swaps {
      public:
        virtual ~batcher3() throw(); //!< cleanup
        explicit batcher3() throw(); //!< setup  
        static const char   sid[];        //!< batcher3
        static const size_t lhs[0x0003];  //!< lhs
        static const size_t rhs[0x0003];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher3);
    };
  }
}
#endif
