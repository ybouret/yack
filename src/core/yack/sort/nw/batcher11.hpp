//! \file
#ifndef YACK_NWSORST_batcher11_INCLUDED
#define YACK_NWSORST_batcher11_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher11
    class batcher11 : public swaps {
      public:
        virtual ~batcher11() noexcept; //!< cleanup
        explicit batcher11() noexcept; //!< setup  
        static const char   sid[];        //!< batcher11
        static const size_t lhs[0x0025];  //!< lhs
        static const size_t rhs[0x0025];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher11);
    };
  }
}
#endif
