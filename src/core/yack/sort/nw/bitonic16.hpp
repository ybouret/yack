//! \file
#ifndef YACK_NWSORST_bitonic16_INCLUDED
#define YACK_NWSORST_bitonic16_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic16
    class bitonic16 : public swaps {
      public:
        virtual ~bitonic16() throw(); //!< cleanup
        explicit bitonic16() throw(); //!< setup  
        static const char   sid[];        //!< bitonic16
        static const size_t lhs[0x0050];  //!< lhs
        static const size_t rhs[0x0050];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic16);
    };
  }
}
#endif
