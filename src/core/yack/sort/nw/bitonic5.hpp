//! \file
#ifndef YACK_NWSORST_bitonic5_INCLUDED
#define YACK_NWSORST_bitonic5_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic5
    class bitonic5 : public pairs<5,0x0009> {
      public:
        virtual ~bitonic5() throw(); //!< cleanup
        explicit bitonic5() throw(); //!< setup  
        static const char   sid[];        //!< bitonic5
        static const size_t lhs[0x0009];  //!< lhs
        static const size_t rhs[0x0009];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic5);
    };
  }
}
#endif
