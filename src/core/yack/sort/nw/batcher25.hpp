//! \file
#ifndef YACK_NWSORST_batcher25_INCLUDED
#define YACK_NWSORST_batcher25_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher25
    class batcher25 : public pairs<25,0x008a> {
      public:
        virtual ~batcher25() throw(); //!< cleanup
        explicit batcher25() throw(); //!< setup  
        static const char   sid[];        //!< batcher25
        static const size_t lhs[0x008a];  //!< lhs
        static const size_t rhs[0x008a];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher25);
    };
  }
}
#endif
