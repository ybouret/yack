//! \file
#ifndef YACK_NWSORST_bosenelson27_INCLUDED
#define YACK_NWSORST_bosenelson27_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson27
    class bosenelson27 : public swaps {
      public:
        virtual ~bosenelson27() noexcept; //!< cleanup
        explicit bosenelson27() noexcept; //!< setup  
        static const char   sid[];        //!< bosenelson27
        static const size_t lhs[0x00ad];  //!< lhs
        static const size_t rhs[0x00ad];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson27);
    };
  }
}
#endif
