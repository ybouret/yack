//! \file
#ifndef YACK_NWSORST_bosenelson32_INCLUDED
#define YACK_NWSORST_bosenelson32_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson32
    class bosenelson32 : public pairs<32,0x00d3> {
      public:
        virtual ~bosenelson32() throw(); //!< cleanup
        explicit bosenelson32() throw(); //!< setup  
        static const char   sid[];        //!< bosenelson32
        static const size_t lhs[0x00d3];  //!< lhs
        static const size_t rhs[0x00d3];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson32);
    };
  }
}
#endif
