//! \file
#ifndef YACK_NWSORST_batcher6_INCLUDED
#define YACK_NWSORST_batcher6_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher6
    class batcher6 : public pairs<6,0x000c> {
      public:
        virtual ~batcher6() throw(); //!< cleanup
        explicit batcher6() throw(); //!< setup  
        static const char   sid[];        //!< batcher6
        static const size_t lhs[0x000c];  //!< lhs
        static const size_t rhs[0x000c];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher6);
    };
  }
}
#endif
