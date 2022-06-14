//! \file
#ifndef YACK_NWSORST_batcher23_INCLUDED
#define YACK_NWSORST_batcher23_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher23
    class batcher23 : public swaps {
      public:
        virtual ~batcher23() throw(); //!< cleanup
        explicit batcher23() throw(); //!< setup  
        static const char   sid[];        //!< batcher23
        static const size_t lhs[0x007a];  //!< lhs
        static const size_t rhs[0x007a];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher23);
    };
  }
}
#endif
