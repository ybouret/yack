//! \file
#ifndef YACK_NWSORST_bosenelson28_INCLUDED
#define YACK_NWSORST_bosenelson28_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson28
    class bosenelson28 : public pairs<28,0x00b3> {
      public:
        virtual ~bosenelson28() throw(); //!< cleanup
        explicit bosenelson28() throw(); //!< setup  
        static const char   sid[];        //!< bosenelson28
        static const size_t lhs[0x00b3];  //!< lhs
        static const size_t rhs[0x00b3];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson28);
    };
  }
}
#endif
