//! \file
#ifndef YACK_NWSORST_bosenelson32_INCLUDED
#define YACK_NWSORST_bosenelson32_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson32
    class bosenelson32 : public swaps {
      public:
        virtual ~bosenelson32() noexcept; //!< cleanup
        explicit bosenelson32() noexcept; //!< setup  
        static const char   sid[];        //!< bosenelson32
        static const size_t lhs[0x00d3];  //!< lhs
        static const size_t rhs[0x00d3];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson32);
    };
  }
}
#endif
