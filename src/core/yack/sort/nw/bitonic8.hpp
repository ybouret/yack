//! \file
#ifndef YACK_NWSORST_bitonic8_INCLUDED
#define YACK_NWSORST_bitonic8_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic8
    class bitonic8 : public swaps {
      public:
        virtual ~bitonic8() noexcept; //!< cleanup
        explicit bitonic8() noexcept; //!< setup  
        static const char   sid[];        //!< bitonic8
        static const size_t lhs[0x0018];  //!< lhs
        static const size_t rhs[0x0018];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic8);
    };
  }
}
#endif
