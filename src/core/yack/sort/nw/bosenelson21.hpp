//! \file
#ifndef YACK_NWSORST_bosenelson21_INCLUDED
#define YACK_NWSORST_bosenelson21_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson21
    class bosenelson21 : public swaps {
      public:
        virtual ~bosenelson21() throw(); //!< cleanup
        explicit bosenelson21() throw(); //!< setup  
        static const char   sid[];        //!< bosenelson21
        static const size_t lhs[0x0076];  //!< lhs
        static const size_t rhs[0x0076];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson21);
    };
  }
}
#endif
