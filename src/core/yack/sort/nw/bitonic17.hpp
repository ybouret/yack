//! \file
#ifndef YACK_NWSORST_bitonic17_INCLUDED
#define YACK_NWSORST_bitonic17_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic17
    class bitonic17 : public swaps {
      public:
        virtual ~bitonic17() noexcept; //!< cleanup
        explicit bitonic17() noexcept; //!< setup  
        static const char   sid[];        //!< bitonic17
        static const size_t lhs[0x0055];  //!< lhs
        static const size_t rhs[0x0055];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic17);
    };
  }
}
#endif
