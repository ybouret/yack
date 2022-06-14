//! \file
#ifndef YACK_NWSORST_bitonic11_INCLUDED
#define YACK_NWSORST_bitonic11_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic11
    class bitonic11 : public swaps {
      public:
        virtual ~bitonic11() throw(); //!< cleanup
        explicit bitonic11() throw(); //!< setup  
        static const char   sid[];        //!< bitonic11
        static const size_t lhs[0x0027];  //!< lhs
        static const size_t rhs[0x0027];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic11);
    };
  }
}
#endif
