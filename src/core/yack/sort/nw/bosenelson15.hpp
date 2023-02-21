//! \file
#ifndef YACK_NWSORST_bosenelson15_INCLUDED
#define YACK_NWSORST_bosenelson15_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson15
    class bosenelson15 : public swaps {
      public:
        virtual ~bosenelson15() noexcept; //!< cleanup
        explicit bosenelson15() noexcept; //!< setup  
        static const char   sid[];        //!< bosenelson15
        static const size_t lhs[0x003d];  //!< lhs
        static const size_t rhs[0x003d];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson15);
    };
  }
}
#endif
