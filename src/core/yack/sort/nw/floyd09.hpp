//! \file
#ifndef YACK_NWSORST_floyd09_INCLUDED
#define YACK_NWSORST_floyd09_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! floyd09
    class floyd09 : public pairs<9,0x0019> {
      public:
        virtual ~floyd09() throw(); //!< cleanup
        explicit floyd09() throw(); //!< setup  
        static const char   sid[];        //!< floyd09
        static const size_t lhs[0x0019];  //!< lhs
        static const size_t rhs[0x0019];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(floyd09);
    };
  }
}
#endif
