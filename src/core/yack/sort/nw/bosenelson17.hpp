//! \file
#ifndef YACK_NWSORST_bosenelson17_INCLUDED
#define YACK_NWSORST_bosenelson17_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson17
    class bosenelson17 : public swaps {
      public:
        virtual ~bosenelson17() noexcept; //!< cleanup
        explicit bosenelson17() noexcept; //!< setup  
        static const char   sid[];        //!< bosenelson17
        static const size_t lhs[0x0051];  //!< lhs
        static const size_t rhs[0x0051];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson17);
    };
  }
}
#endif
