//! \file
#ifndef YACK_NWSORST_batcher16_INCLUDED
#define YACK_NWSORST_batcher16_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! batcher16
    class batcher16 : public swaps {
      public:
        virtual ~batcher16() noexcept; //!< cleanup
        explicit batcher16() noexcept; //!< setup  
        static const char   sid[];        //!< batcher16
        static const size_t lhs[0x003f];  //!< lhs
        static const size_t rhs[0x003f];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(batcher16);
    };
  }
}
#endif
