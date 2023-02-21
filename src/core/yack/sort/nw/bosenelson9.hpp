//! \file
#ifndef YACK_NWSORST_bosenelson9_INCLUDED
#define YACK_NWSORST_bosenelson9_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson9
    class bosenelson9 : public swaps {
      public:
        virtual ~bosenelson9() noexcept; //!< cleanup
        explicit bosenelson9() noexcept; //!< setup  
        static const char   sid[];        //!< bosenelson9
        static const size_t lhs[0x001b];  //!< lhs
        static const size_t rhs[0x001b];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson9);
    };
  }
}
#endif
