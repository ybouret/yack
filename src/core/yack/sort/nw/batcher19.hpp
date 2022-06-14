//! \file
#ifndef YACK_NWSORST_batcher19_INCLUDED
#define YACK_NWSORST_batcher19_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher19
    class batcher19 : public swaps {
      public:
        virtual ~batcher19() throw(); //!< cleanup
        explicit batcher19() throw(); //!< setup  
        static const char   sid[];        //!< batcher19
        static const size_t lhs[0x005b];  //!< lhs
        static const size_t rhs[0x005b];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher19);
    };
  }
}
#endif
