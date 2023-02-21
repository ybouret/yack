//! \file
#ifndef YACK_NWSORST_bosenelson2_INCLUDED
#define YACK_NWSORST_bosenelson2_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson2
    class bosenelson2 : public swaps {
      public:
        virtual ~bosenelson2() noexcept; //!< cleanup
        explicit bosenelson2() noexcept; //!< setup  
        static const char   sid[];        //!< bosenelson2
        static const size_t lhs[0x0001];  //!< lhs
        static const size_t rhs[0x0001];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson2);
    };
  }
}
#endif
