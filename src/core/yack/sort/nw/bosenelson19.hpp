//! \file
#ifndef YACK_NWSORST_bosenelson19_INCLUDED
#define YACK_NWSORST_bosenelson19_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson19
    class bosenelson19 : public swaps {
      public:
        virtual ~bosenelson19() throw(); //!< cleanup
        explicit bosenelson19() throw(); //!< setup  
        static const char   sid[];        //!< bosenelson19
        static const size_t lhs[0x0064];  //!< lhs
        static const size_t rhs[0x0064];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson19);
    };
  }
}
#endif
