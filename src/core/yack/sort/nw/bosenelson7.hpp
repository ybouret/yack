//! \file
#ifndef YACK_NWSORST_bosenelson7_INCLUDED
#define YACK_NWSORST_bosenelson7_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! bosenelson7
    class bosenelson7 : public pairs<7,0x0010> {
      public:
        virtual ~bosenelson7() throw(); //!< cleanup
        explicit bosenelson7() throw(); //!< setup  
        static const char   sid[];        //!< bosenelson7
        static const size_t lhs[0x0010];  //!< lhs
        static const size_t rhs[0x0010];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(bosenelson7);
    };
  }
}
#endif
