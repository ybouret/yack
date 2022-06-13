//! \file
#ifndef YACK_NWSORST_bosenelson4_INCLUDED
#define YACK_NWSORST_bosenelson4_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson4
    class bosenelson4 : public pairs<4,0x0005> {
      public:
        virtual ~bosenelson4() throw(); //!< cleanup
        explicit bosenelson4() throw(); //!< setup  
        static const char   sid[];        //!< bosenelson4
        static const size_t lhs[0x0005];  //!< lhs
        static const size_t rhs[0x0005];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson4);
    };
  }
}
#endif
