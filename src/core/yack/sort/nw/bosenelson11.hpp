//! \file
#ifndef YACK_NWSORST_bosenelson11_INCLUDED
#define YACK_NWSORST_bosenelson11_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson11
    class bosenelson11 : public swaps {
      public:
        virtual ~bosenelson11() noexcept; //!< cleanup
        explicit bosenelson11() noexcept; //!< setup  
        static const char   sid[];        //!< bosenelson11
        static const size_t lhs[0x0026];  //!< lhs
        static const size_t rhs[0x0026];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson11);
    };
  }
}
#endif
