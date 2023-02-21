//! \file
#ifndef YACK_NWSORST_bosenelson24_INCLUDED
#define YACK_NWSORST_bosenelson24_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson24
    class bosenelson24 : public swaps {
      public:
        virtual ~bosenelson24() noexcept; //!< cleanup
        explicit bosenelson24() noexcept; //!< setup  
        static const char   sid[];        //!< bosenelson24
        static const size_t lhs[0x008a];  //!< lhs
        static const size_t rhs[0x008a];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson24);
    };
  }
}
#endif
