//! \file
#ifndef YACK_NWSORST_bitonic4_INCLUDED
#define YACK_NWSORST_bitonic4_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic4
    class bitonic4 : public pairs<4,0x0006> {
      public:
        virtual ~bitonic4() throw(); //!< cleanup
        explicit bitonic4() throw(); //!< setup  
        static const char   sid[];        //!< bitonic4
        static const size_t lhs[0x0006];  //!< lhs
        static const size_t rhs[0x0006];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic4);
    };
  }
}
#endif
