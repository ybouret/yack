//! \file
#ifndef YACK_NWSORST_bosenelson5_INCLUDED
#define YACK_NWSORST_bosenelson5_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson5
    class bosenelson5 : public swaps {
      public:
        virtual ~bosenelson5() noexcept; //!< cleanup
        explicit bosenelson5() noexcept; //!< setup  
        static const char   sid[];        //!< bosenelson5
        static const size_t lhs[0x0009];  //!< lhs
        static const size_t rhs[0x0009];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson5);
    };
  }
}
#endif
