//! \file
#ifndef YACK_NWSORST_bosenelson22_INCLUDED
#define YACK_NWSORST_bosenelson22_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson22
    class bosenelson22 : public swaps {
      public:
        virtual ~bosenelson22() throw(); //!< cleanup
        explicit bosenelson22() throw(); //!< setup  
        static const char   sid[];        //!< bosenelson22
        static const size_t lhs[0x007d];  //!< lhs
        static const size_t rhs[0x007d];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson22);
    };
  }
}
#endif
