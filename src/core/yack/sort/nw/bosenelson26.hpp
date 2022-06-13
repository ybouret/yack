//! \file
#ifndef YACK_NWSORST_bosenelson26_INCLUDED
#define YACK_NWSORST_bosenelson26_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson26
    class bosenelson26 : public swaps {
      public:
        virtual ~bosenelson26() throw(); //!< cleanup
        explicit bosenelson26() throw(); //!< setup  
        static const char   sid[];        //!< bosenelson26
        static const size_t lhs[0x00a3];  //!< lhs
        static const size_t rhs[0x00a3];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson26);
    };
  }
}
#endif
