//! \file
#ifndef YACK_NWSORST_batcher20_INCLUDED
#define YACK_NWSORST_batcher20_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher20
    class batcher20 : public swaps {
      public:
        virtual ~batcher20() throw(); //!< cleanup
        explicit batcher20() throw(); //!< setup  
        static const char   sid[];        //!< batcher20
        static const size_t lhs[0x0061];  //!< lhs
        static const size_t rhs[0x0061];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher20);
    };
  }
}
#endif
