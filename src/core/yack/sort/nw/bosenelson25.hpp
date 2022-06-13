//! \file
#ifndef YACK_NWSORST_bosenelson25_INCLUDED
#define YACK_NWSORST_bosenelson25_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson25
    class bosenelson25 : public pairs<25,0x009a> {
      public:
        virtual ~bosenelson25() throw(); //!< cleanup
        explicit bosenelson25() throw(); //!< setup  
        static const char   sid[];        //!< bosenelson25
        static const size_t lhs[0x009a];  //!< lhs
        static const size_t rhs[0x009a];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson25);
    };
  }
}
#endif
