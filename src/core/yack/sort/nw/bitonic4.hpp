//! \file
#ifndef YACK_NWSORST_bitonic4_INCLUDED
#define YACK_NWSORST_bitonic4_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic4
    class bitonic4 : public swaps {
      public:
        virtual ~bitonic4() noexcept; //!< cleanup
        explicit bitonic4() noexcept; //!< setup  
        static const char   sid[];        //!< bitonic4
        static const size_t lhs[0x0006];  //!< lhs
        static const size_t rhs[0x0006];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic4);
    };
  }
}
#endif
