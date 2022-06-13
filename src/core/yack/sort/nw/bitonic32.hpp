//! \file
#ifndef YACK_NWSORST_bitonic32_INCLUDED
#define YACK_NWSORST_bitonic32_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic32
    class bitonic32 : public swaps {
      public:
        virtual ~bitonic32() throw(); //!< cleanup
        explicit bitonic32() throw(); //!< setup  
        static const char   sid[];        //!< bitonic32
        static const size_t lhs[0x00f0];  //!< lhs
        static const size_t rhs[0x00f0];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic32);
    };
  }
}
#endif
