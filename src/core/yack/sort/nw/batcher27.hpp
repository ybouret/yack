//! \file
#ifndef YACK_NWSORST_batcher27_INCLUDED
#define YACK_NWSORST_batcher27_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher27
    class batcher27 : public pairs<27,0x009b> {
      public:
        virtual ~batcher27() throw(); //!< cleanup
        explicit batcher27() throw(); //!< setup  
        static const char   sid[];        //!< batcher27
        static const size_t lhs[0x009b];  //!< lhs
        static const size_t rhs[0x009b];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher27);
    };
  }
}
#endif
