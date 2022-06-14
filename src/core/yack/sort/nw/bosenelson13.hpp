//! \file
#ifndef YACK_NWSORST_bosenelson13_INCLUDED
#define YACK_NWSORST_bosenelson13_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson13
    class bosenelson13 : public swaps {
      public:
        virtual ~bosenelson13() throw(); //!< cleanup
        explicit bosenelson13() throw(); //!< setup  
        static const char   sid[];        //!< bosenelson13
        static const size_t lhs[0x0032];  //!< lhs
        static const size_t rhs[0x0032];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson13);
    };
  }
}
#endif
