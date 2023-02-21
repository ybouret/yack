//! \file
#ifndef YACK_NWSORST_bosenelson28_INCLUDED
#define YACK_NWSORST_bosenelson28_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson28
    class bosenelson28 : public swaps {
      public:
        virtual ~bosenelson28() noexcept; //!< cleanup
        explicit bosenelson28() noexcept; //!< setup  
        static const char   sid[];        //!< bosenelson28
        static const size_t lhs[0x00b3];  //!< lhs
        static const size_t rhs[0x00b3];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson28);
    };
  }
}
#endif
