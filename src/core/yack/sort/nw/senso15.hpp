//! \file
#ifndef YACK_NWSORST_senso15_INCLUDED
#define YACK_NWSORST_senso15_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! senso15
    class senso15 : public swaps {
      public:
        virtual ~senso15() noexcept; //!< cleanup
        explicit senso15() noexcept; //!< setup  
        static const char   sid[];        //!< senso15
        static const size_t lhs[0x0038];  //!< lhs
        static const size_t rhs[0x0038];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(senso15);
    };
  }
}
#endif
