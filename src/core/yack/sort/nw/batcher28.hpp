//! \file
#ifndef YACK_NWSORST_batcher28_INCLUDED
#define YACK_NWSORST_batcher28_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher28
    class batcher28 : public pairs<28,0x00a1> {
      public:
        virtual ~batcher28() throw(); //!< cleanup
        explicit batcher28() throw(); //!< setup  
        static const char   sid[];        //!< batcher28
        static const size_t lhs[0x00a1];  //!< lhs
        static const size_t rhs[0x00a1];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher28);
    };
  }
}
#endif
