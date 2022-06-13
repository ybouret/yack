//! \file
#ifndef YACK_NWSORST_bosenelson6_INCLUDED
#define YACK_NWSORST_bosenelson6_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson6
    class bosenelson6 : public swaps {
      public:
        virtual ~bosenelson6() throw(); //!< cleanup
        explicit bosenelson6() throw(); //!< setup  
        static const char   sid[];        //!< bosenelson6
        static const size_t lhs[0x000c];  //!< lhs
        static const size_t rhs[0x000c];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson6);
    };
  }
}
#endif
