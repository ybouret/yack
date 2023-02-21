//! \file
#ifndef YACK_NWSORST_bitonic18_INCLUDED
#define YACK_NWSORST_bitonic18_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic18
    class bitonic18 : public swaps {
      public:
        virtual ~bitonic18() noexcept; //!< cleanup
        explicit bitonic18() noexcept; //!< setup  
        static const char   sid[];        //!< bitonic18
        static const size_t lhs[0x005b];  //!< lhs
        static const size_t rhs[0x005b];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic18);
    };
  }
}
#endif
