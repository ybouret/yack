//! \file
#ifndef YACK_NWSORST_bitonic27_INCLUDED
#define YACK_NWSORST_bitonic27_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic27
    class bitonic27 : public swaps {
      public:
        virtual ~bitonic27() noexcept; //!< cleanup
        explicit bitonic27() noexcept; //!< setup  
        static const char   sid[];        //!< bitonic27
        static const size_t lhs[0x00ae];  //!< lhs
        static const size_t rhs[0x00ae];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic27);
    };
  }
}
#endif
