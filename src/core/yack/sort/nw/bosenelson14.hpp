//! \file
#ifndef YACK_NWSORST_bosenelson14_INCLUDED
#define YACK_NWSORST_bosenelson14_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson14
    class bosenelson14 : public swaps {
      public:
        virtual ~bosenelson14() throw(); //!< cleanup
        explicit bosenelson14() throw(); //!< setup  
        static const char   sid[];        //!< bosenelson14
        static const size_t lhs[0x0037];  //!< lhs
        static const size_t rhs[0x0037];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson14);
    };
  }
}
#endif
