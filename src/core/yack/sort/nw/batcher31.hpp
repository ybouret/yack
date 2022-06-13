//! \file
#ifndef YACK_NWSORST_batcher31_INCLUDED
#define YACK_NWSORST_batcher31_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher31
    class batcher31 : public swaps {
      public:
        virtual ~batcher31() throw(); //!< cleanup
        explicit batcher31() throw(); //!< setup  
        static const char   sid[];        //!< batcher31
        static const size_t lhs[0x00ba];  //!< lhs
        static const size_t rhs[0x00ba];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher31);
    };
  }
}
#endif
