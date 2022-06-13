//! \file
#ifndef YACK_NWSORST_batcher7_INCLUDED
#define YACK_NWSORST_batcher7_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher7
    class batcher7 : public pairs<7,0x0010> {
      public:
        virtual ~batcher7() throw(); //!< cleanup
        explicit batcher7() throw(); //!< setup  
        static const char   sid[];        //!< batcher7
        static const size_t lhs[0x0010];  //!< lhs
        static const size_t rhs[0x0010];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher7);
    };
  }
}
#endif
