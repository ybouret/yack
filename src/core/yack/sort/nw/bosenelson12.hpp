//! \file
#ifndef YACK_NWSORST_bosenelson12_INCLUDED
#define YACK_NWSORST_bosenelson12_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson12
    class bosenelson12 : public swaps {
      public:
        virtual ~bosenelson12() noexcept; //!< cleanup
        explicit bosenelson12() noexcept; //!< setup  
        static const char   sid[];        //!< bosenelson12
        static const size_t lhs[0x002a];  //!< lhs
        static const size_t rhs[0x002a];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson12);
    };
  }
}
#endif
