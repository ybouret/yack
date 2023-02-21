//! \file
#ifndef YACK_NWSORST_bosenelson29_INCLUDED
#define YACK_NWSORST_bosenelson29_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson29
    class bosenelson29 : public swaps {
      public:
        virtual ~bosenelson29() noexcept; //!< cleanup
        explicit bosenelson29() noexcept; //!< setup  
        static const char   sid[];        //!< bosenelson29
        static const size_t lhs[0x00bf];  //!< lhs
        static const size_t rhs[0x00bf];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson29);
    };
  }
}
#endif
