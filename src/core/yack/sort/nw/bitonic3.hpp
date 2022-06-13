//! \file
#ifndef YACK_NWSORST_bitonic3_INCLUDED
#define YACK_NWSORST_bitonic3_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic3
    class bitonic3 : public swaps {
      public:
        virtual ~bitonic3() throw(); //!< cleanup
        explicit bitonic3() throw(); //!< setup  
        static const char   sid[];        //!< bitonic3
        static const size_t lhs[0x0003];  //!< lhs
        static const size_t rhs[0x0003];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic3);
    };
  }
}
#endif
