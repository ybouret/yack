//! \file
#ifndef YACK_NWSORST_batcher30_INCLUDED
#define YACK_NWSORST_batcher30_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher30
    class batcher30 : public swaps {
      public:
        virtual ~batcher30() throw(); //!< cleanup
        explicit batcher30() throw(); //!< setup  
        static const char   sid[];        //!< batcher30
        static const size_t lhs[0x00b2];  //!< lhs
        static const size_t rhs[0x00b2];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher30);
    };
  }
}
#endif
