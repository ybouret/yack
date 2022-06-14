//! \file
#ifndef YACK_NWSORST_bosenelson20_INCLUDED
#define YACK_NWSORST_bosenelson20_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson20
    class bosenelson20 : public swaps {
      public:
        virtual ~bosenelson20() throw(); //!< cleanup
        explicit bosenelson20() throw(); //!< setup  
        static const char   sid[];        //!< bosenelson20
        static const size_t lhs[0x006a];  //!< lhs
        static const size_t rhs[0x006a];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson20);
    };
  }
}
#endif
