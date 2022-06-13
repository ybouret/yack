//! \file
#ifndef YACK_NWSORST_bosenelson30_INCLUDED
#define YACK_NWSORST_bosenelson30_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson30
    class bosenelson30 : public swaps {
      public:
        virtual ~bosenelson30() throw(); //!< cleanup
        explicit bosenelson30() throw(); //!< setup  
        static const char   sid[];        //!< bosenelson30
        static const size_t lhs[0x00c6];  //!< lhs
        static const size_t rhs[0x00c6];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson30);
    };
  }
}
#endif
