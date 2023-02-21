//! \file
#ifndef YACK_NWSORST_bitonic29_INCLUDED
#define YACK_NWSORST_bitonic29_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic29
    class bitonic29 : public swaps {
      public:
        virtual ~bitonic29() noexcept; //!< cleanup
        explicit bitonic29() noexcept; //!< setup  
        static const char   sid[];        //!< bitonic29
        static const size_t lhs[0x00c6];  //!< lhs
        static const size_t rhs[0x00c6];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic29);
    };
  }
}
#endif
