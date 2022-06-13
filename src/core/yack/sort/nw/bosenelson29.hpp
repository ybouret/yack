//! \file
#ifndef YACK_NWSORST_bosenelson29_INCLUDED
#define YACK_NWSORST_bosenelson29_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson29
    class bosenelson29 : public pairs<29,0x00bf> {
      public:
        virtual ~bosenelson29() throw(); //!< cleanup
        explicit bosenelson29() throw(); //!< setup  
        static const char   sid[];        //!< bosenelson29
        static const size_t lhs[0x00bf];  //!< lhs
        static const size_t rhs[0x00bf];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson29);
    };
  }
}
#endif
