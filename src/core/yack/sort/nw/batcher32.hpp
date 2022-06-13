//! \file
#ifndef YACK_NWSORST_batcher32_INCLUDED
#define YACK_NWSORST_batcher32_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher32
    class batcher32 : public pairs<32,0x00bf> {
      public:
        virtual ~batcher32() throw(); //!< cleanup
        explicit batcher32() throw(); //!< setup  
        static const char   sid[];        //!< batcher32
        static const size_t lhs[0x00bf];  //!< lhs
        static const size_t rhs[0x00bf];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher32);
    };
  }
}
#endif
