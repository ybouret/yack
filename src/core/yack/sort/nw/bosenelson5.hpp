//! \file
#ifndef YACK_NWSORST_bosenelson5_INCLUDED
#define YACK_NWSORST_bosenelson5_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson5
    class bosenelson5 : public pairs<5,0x0009> {
      public:
        virtual ~bosenelson5() throw(); //!< cleanup
        explicit bosenelson5() throw(); //!< setup  
        static const char   sid[];        //!< bosenelson5
        static const size_t lhs[0x0009];  //!< lhs
        static const size_t rhs[0x0009];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson5);
    };
  }
}
#endif
