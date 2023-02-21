//! \file
#ifndef YACK_NWSORST_bitonic14_INCLUDED
#define YACK_NWSORST_bitonic14_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bitonic14
    class bitonic14 : public swaps {
      public:
        virtual ~bitonic14() noexcept; //!< cleanup
        explicit bitonic14() noexcept; //!< setup  
        static const char   sid[];        //!< bitonic14
        static const size_t lhs[0x003d];  //!< lhs
        static const size_t rhs[0x003d];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bitonic14);
    };
  }
}
#endif
