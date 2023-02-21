//! \file
#ifndef YACK_NWSORST_batcher4_INCLUDED
#define YACK_NWSORST_batcher4_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher4
    class batcher4 : public swaps {
      public:
        virtual ~batcher4() noexcept; //!< cleanup
        explicit batcher4() noexcept; //!< setup  
        static const char   sid[];        //!< batcher4
        static const size_t lhs[0x0005];  //!< lhs
        static const size_t rhs[0x0005];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher4);
    };
  }
}
#endif
