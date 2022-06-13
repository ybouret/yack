//! \file
#ifndef YACK_NWSORST_bitonic6_INCLUDED
#define YACK_NWSORST_bitonic6_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic6
    class bitonic6 : public swaps {
      public:
        virtual ~bitonic6() throw(); //!< cleanup
        explicit bitonic6() throw(); //!< setup  
        static const char   sid[];        //!< bitonic6
        static const size_t lhs[0x000d];  //!< lhs
        static const size_t rhs[0x000d];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic6);
    };
  }
}
#endif
