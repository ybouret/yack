//! \file
#ifndef YACK_NWSORST_bitonic2_INCLUDED
#define YACK_NWSORST_bitonic2_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic2
    class bitonic2 : public swaps {
      public:
        virtual ~bitonic2() throw(); //!< cleanup
        explicit bitonic2() throw(); //!< setup  
        static const char   sid[];        //!< bitonic2
        static const size_t lhs[0x0001];  //!< lhs
        static const size_t rhs[0x0001];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic2);
    };
  }
}
#endif
