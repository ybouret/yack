//! \file
#ifndef YACK_NWSORST_bitonic13_INCLUDED
#define YACK_NWSORST_bitonic13_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic13
    class bitonic13 : public swaps {
      public:
        virtual ~bitonic13() noexcept; //!< cleanup
        explicit bitonic13() noexcept; //!< setup  
        static const char   sid[];        //!< bitonic13
        static const size_t lhs[0x0035];  //!< lhs
        static const size_t rhs[0x0035];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic13);
    };
  }
}
#endif
