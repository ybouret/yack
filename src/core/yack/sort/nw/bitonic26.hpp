//! \file
#ifndef YACK_NWSORST_bitonic26_INCLUDED
#define YACK_NWSORST_bitonic26_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic26
    class bitonic26 : public swaps {
      public:
        virtual ~bitonic26() noexcept; //!< cleanup
        explicit bitonic26() noexcept; //!< setup  
        static const char   sid[];        //!< bitonic26
        static const size_t lhs[0x00a3];  //!< lhs
        static const size_t rhs[0x00a3];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic26);
    };
  }
}
#endif
