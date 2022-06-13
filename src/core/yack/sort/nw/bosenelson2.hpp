//! \file
#ifndef YACK_NWSORST_bosenelson2_INCLUDED
#define YACK_NWSORST_bosenelson2_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson2
    class bosenelson2 : public pairs<2,0x0001> {
      public:
        virtual ~bosenelson2() throw(); //!< cleanup
        explicit bosenelson2() throw(); //!< setup  
        static const char   sid[];        //!< bosenelson2
        static const size_t lhs[0x0001];  //!< lhs
        static const size_t rhs[0x0001];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson2);
    };
  }
}
#endif
