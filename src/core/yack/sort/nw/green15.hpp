//! \file
#ifndef YACK_NWSORST_green15_INCLUDED
#define YACK_NWSORST_green15_INCLUDED
#include "yack/sort/nw/swaps.hpp"
namespace yack {
  namespace nwsrt {
    //! green15
    class green15 : public pairs<15,0x0038> {
      public:
        virtual ~green15() throw(); //!< cleanup
        explicit green15() throw(); //!< setup  
        static const char   sid[];        //!< green15
        static const size_t lhs[0x0038];  //!< lhs
        static const size_t rhs[0x0038];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(green15);
    };
  }
}
#endif
