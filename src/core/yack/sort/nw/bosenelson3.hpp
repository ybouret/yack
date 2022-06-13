//! \file
#ifndef YACK_NWSORST_bosenelson3_INCLUDED
#define YACK_NWSORST_bosenelson3_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson3
    class bosenelson3 : public pairs<3,0x0003> {
      public:
        virtual ~bosenelson3() throw(); //!< cleanup
        explicit bosenelson3() throw(); //!< setup  
        static const char   sid[];        //!< bosenelson3
        static const size_t lhs[0x0003];  //!< lhs
        static const size_t rhs[0x0003];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson3);
    };
  }
}
#endif
