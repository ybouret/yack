//! \file
#ifndef YACK_NWSORST_bitonic10_INCLUDED
#define YACK_NWSORST_bitonic10_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic10
    class bitonic10 : public swaps {
      public:
        virtual ~bitonic10() throw(); //!< cleanup
        explicit bitonic10() throw(); //!< setup  
        static const char   sid[];        //!< bitonic10
        static const size_t lhs[0x0021];  //!< lhs
        static const size_t rhs[0x0021];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic10);
    };
  }
}
#endif
