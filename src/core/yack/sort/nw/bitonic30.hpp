//! \file
#ifndef YACK_NWSORST_bitonic30_INCLUDED
#define YACK_NWSORST_bitonic30_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic30
    class bitonic30 : public swaps {
      public:
        virtual ~bitonic30() throw(); //!< cleanup
        explicit bitonic30() throw(); //!< setup  
        static const char   sid[];        //!< bitonic30
        static const size_t lhs[0x00d3];  //!< lhs
        static const size_t rhs[0x00d3];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic30);
    };
  }
}
#endif
