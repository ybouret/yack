//! \file
#ifndef YACK_NWSORST_bitonic15_INCLUDED
#define YACK_NWSORST_bitonic15_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic15
    class bitonic15 : public swaps {
      public:
        virtual ~bitonic15() noexcept; //!< cleanup
        explicit bitonic15() noexcept; //!< setup  
        static const char   sid[];        //!< bitonic15
        static const size_t lhs[0x0046];  //!< lhs
        static const size_t rhs[0x0046];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic15);
    };
  }
}
#endif
