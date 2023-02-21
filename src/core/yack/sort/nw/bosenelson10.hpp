//! \file
#ifndef YACK_NWSORST_bosenelson10_INCLUDED
#define YACK_NWSORST_bosenelson10_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson10
    class bosenelson10 : public swaps {
      public:
        virtual ~bosenelson10() noexcept; //!< cleanup
        explicit bosenelson10() noexcept; //!< setup  
        static const char   sid[];        //!< bosenelson10
        static const size_t lhs[0x0020];  //!< lhs
        static const size_t rhs[0x0020];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson10);
    };
  }
}
#endif
