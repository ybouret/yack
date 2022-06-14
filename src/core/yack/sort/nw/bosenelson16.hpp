//! \file
#ifndef YACK_NWSORST_bosenelson16_INCLUDED
#define YACK_NWSORST_bosenelson16_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson16
    class bosenelson16 : public swaps {
      public:
        virtual ~bosenelson16() throw(); //!< cleanup
        explicit bosenelson16() throw(); //!< setup  
        static const char   sid[];        //!< bosenelson16
        static const size_t lhs[0x0041];  //!< lhs
        static const size_t rhs[0x0041];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson16);
    };
  }
}
#endif
