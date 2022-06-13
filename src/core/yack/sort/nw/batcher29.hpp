//! \file
#ifndef YACK_NWSORST_batcher29_INCLUDED
#define YACK_NWSORST_batcher29_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher29
    class batcher29 : public swaps {
      public:
        virtual ~batcher29() throw(); //!< cleanup
        explicit batcher29() throw(); //!< setup  
        static const char   sid[];        //!< batcher29
        static const size_t lhs[0x00ab];  //!< lhs
        static const size_t rhs[0x00ab];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher29);
    };
  }
}
#endif
