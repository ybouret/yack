//! \file
#ifndef YACK_NWSORST_bitonic21_INCLUDED
#define YACK_NWSORST_bitonic21_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic21
    class bitonic21 : public swaps {
      public:
        virtual ~bitonic21() throw(); //!< cleanup
        explicit bitonic21() throw(); //!< setup  
        static const char   sid[];        //!< bitonic21
        static const size_t lhs[0x0072];  //!< lhs
        static const size_t rhs[0x0072];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic21);
    };
  }
}
#endif
