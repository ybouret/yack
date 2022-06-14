//! \file
#ifndef YACK_NWSORST_bosenelson23_INCLUDED
#define YACK_NWSORST_bosenelson23_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson23
    class bosenelson23 : public swaps {
      public:
        virtual ~bosenelson23() throw(); //!< cleanup
        explicit bosenelson23() throw(); //!< setup  
        static const char   sid[];        //!< bosenelson23
        static const size_t lhs[0x0085];  //!< lhs
        static const size_t rhs[0x0085];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson23);
    };
  }
}
#endif
