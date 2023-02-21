//! \file
#ifndef YACK_NWSORST_bitonic20_INCLUDED
#define YACK_NWSORST_bitonic20_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic20
    class bitonic20 : public swaps {
      public:
        virtual ~bitonic20() noexcept; //!< cleanup
        explicit bitonic20() noexcept; //!< setup  
        static const char   sid[];        //!< bitonic20
        static const size_t lhs[0x006a];  //!< lhs
        static const size_t rhs[0x006a];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic20);
    };
  }
}
#endif
