//! \file
#ifndef YACK_NWSORST_bosenelson18_INCLUDED
#define YACK_NWSORST_bosenelson18_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson18
    class bosenelson18 : public swaps {
      public:
        virtual ~bosenelson18() throw(); //!< cleanup
        explicit bosenelson18() throw(); //!< setup  
        static const char   sid[];        //!< bosenelson18
        static const size_t lhs[0x005a];  //!< lhs
        static const size_t rhs[0x005a];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson18);
    };
  }
}
#endif
