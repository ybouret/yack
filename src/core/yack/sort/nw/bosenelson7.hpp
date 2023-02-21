//! \file
#ifndef YACK_NWSORST_bosenelson7_INCLUDED
#define YACK_NWSORST_bosenelson7_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson7
    class bosenelson7 : public swaps {
      public:
        virtual ~bosenelson7() noexcept; //!< cleanup
        explicit bosenelson7() noexcept; //!< setup  
        static const char   sid[];        //!< bosenelson7
        static const size_t lhs[0x0010];  //!< lhs
        static const size_t rhs[0x0010];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson7);
    };
  }
}
#endif
