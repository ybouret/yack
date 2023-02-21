//! \file
#ifndef YACK_NWSORST_bitonic24_INCLUDED
#define YACK_NWSORST_bitonic24_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic24
    class bitonic24 : public swaps {
      public:
        virtual ~bitonic24() noexcept; //!< cleanup
        explicit bitonic24() noexcept; //!< setup  
        static const char   sid[];        //!< bitonic24
        static const size_t lhs[0x0090];  //!< lhs
        static const size_t rhs[0x0090];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic24);
    };
  }
}
#endif
