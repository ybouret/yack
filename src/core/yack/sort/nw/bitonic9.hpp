//! \file
#ifndef YACK_NWSORST_bitonic9_INCLUDED
#define YACK_NWSORST_bitonic9_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic9
    class bitonic9 : public swaps {
      public:
        virtual ~bitonic9() throw(); //!< cleanup
        explicit bitonic9() throw(); //!< setup  
        static const char   sid[];        //!< bitonic9
        static const size_t lhs[0x001c];  //!< lhs
        static const size_t rhs[0x001c];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic9);
    };
  }
}
#endif
