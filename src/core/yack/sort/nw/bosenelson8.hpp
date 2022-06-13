//! \file
#ifndef YACK_NWSORST_bosenelson8_INCLUDED
#define YACK_NWSORST_bosenelson8_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson8
    class bosenelson8 : public pairs<8,0x0013> {
      public:
        virtual ~bosenelson8() throw(); //!< cleanup
        explicit bosenelson8() throw(); //!< setup  
        static const char   sid[];        //!< bosenelson8
        static const size_t lhs[0x0013];  //!< lhs
        static const size_t rhs[0x0013];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson8);
    };
  }
}
#endif
