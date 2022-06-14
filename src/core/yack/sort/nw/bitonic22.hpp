//! \file
#ifndef YACK_NWSORST_bitonic22_INCLUDED
#define YACK_NWSORST_bitonic22_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic22
    class bitonic22 : public swaps {
      public:
        virtual ~bitonic22() throw(); //!< cleanup
        explicit bitonic22() throw(); //!< setup  
        static const char   sid[];        //!< bitonic22
        static const size_t lhs[0x007b];  //!< lhs
        static const size_t rhs[0x007b];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic22);
    };
  }
}
#endif
