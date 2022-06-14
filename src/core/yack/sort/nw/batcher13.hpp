//! \file
#ifndef YACK_NWSORST_batcher13_INCLUDED
#define YACK_NWSORST_batcher13_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher13
    class batcher13 : public swaps {
      public:
        virtual ~batcher13() throw(); //!< cleanup
        explicit batcher13() throw(); //!< setup  
        static const char   sid[];        //!< batcher13
        static const size_t lhs[0x0030];  //!< lhs
        static const size_t rhs[0x0030];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher13);
    };
  }
}
#endif
