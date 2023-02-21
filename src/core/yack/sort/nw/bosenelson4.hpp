//! \file
#ifndef YACK_NWSORST_bosenelson4_INCLUDED
#define YACK_NWSORST_bosenelson4_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson4
    class bosenelson4 : public swaps {
      public:
        virtual ~bosenelson4() noexcept; //!< cleanup
        explicit bosenelson4() noexcept; //!< setup  
        static const char   sid[];        //!< bosenelson4
        static const size_t lhs[0x0005];  //!< lhs
        static const size_t rhs[0x0005];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson4);
    };
  }
}
#endif
