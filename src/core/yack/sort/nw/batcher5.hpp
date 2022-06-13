//! \file
#ifndef YACK_NWSORST_batcher5_INCLUDED
#define YACK_NWSORST_batcher5_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher5
    class batcher5 : public swaps {
      public:
        virtual ~batcher5() throw(); //!< cleanup
        explicit batcher5() throw(); //!< setup  
        static const char   sid[];        //!< batcher5
        static const size_t lhs[0x0009];  //!< lhs
        static const size_t rhs[0x0009];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher5);
    };
  }
}
#endif
