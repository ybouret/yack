//! \file
#ifndef YACK_NWSORST_batcher2_INCLUDED
#define YACK_NWSORST_batcher2_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher2
    class batcher2 : public pairs<2,0x0001> {
      public:
        virtual ~batcher2() throw(); //!< cleanup
        explicit batcher2() throw(); //!< setup  
        static const char   sid[];        //!< batcher2
        static const size_t lhs[0x0001];  //!< lhs
        static const size_t rhs[0x0001];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher2);
    };
  }
}
#endif
