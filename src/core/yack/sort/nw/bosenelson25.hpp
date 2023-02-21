//! \file
#ifndef YACK_NWSORST_bosenelson25_INCLUDED
#define YACK_NWSORST_bosenelson25_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson25
    class bosenelson25 : public swaps {
      public:
        virtual ~bosenelson25() noexcept; //!< cleanup
        explicit bosenelson25() noexcept; //!< setup  
        static const char   sid[];        //!< bosenelson25
        static const size_t lhs[0x009a];  //!< lhs
        static const size_t rhs[0x009a];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson25);
    };
  }
}
#endif
