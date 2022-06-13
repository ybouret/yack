//! \file
#ifndef YACK_NWSORST_bitonic3_INCLUDED
#define YACK_NWSORST_bitonic3_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic3
    class bitonic3 : public pairs<3,0x0003> {
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
