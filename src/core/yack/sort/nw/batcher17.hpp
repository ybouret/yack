//! \file
#ifndef YACK_NWSORST_batcher17_INCLUDED
#define YACK_NWSORST_batcher17_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher17
    class batcher17 : public swaps {
      public:
        virtual ~batcher17() throw(); //!< cleanup
        explicit batcher17() throw(); //!< setup  
        static const char   sid[];        //!< batcher17
        static const size_t lhs[0x004a];  //!< lhs
        static const size_t rhs[0x004a];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher17);
    };
  }
}
#endif
