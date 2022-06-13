//! \file
#ifndef YACK_NWSORST_bitonic8_INCLUDED
#define YACK_NWSORST_bitonic8_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic8
    class bitonic8 : public pairs<8,0x0018> {
      public:
        virtual ~bitonic8() throw(); //!< cleanup
        explicit bitonic8() throw(); //!< setup  
        static const char   sid[];        //!< bitonic8
        static const size_t lhs[0x0018];  //!< lhs
        static const size_t rhs[0x0018];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic8);
    };
  }
}
#endif
