//! \file
#ifndef YACK_NWSORST_senso20_INCLUDED
#define YACK_NWSORST_senso20_INCLUDED 1
#include "yack/sort/nwswp.hpp"
namespace yack {
  namespace nwsrt {
    //! senso20
    class senso20 : public swaps {
      public:
        virtual ~senso20() noexcept; //!< cleanup
        explicit senso20() noexcept; //!< setup  
        static const char   sid[];        //!< senso20
        static const size_t lhs[0x005c];  //!< lhs
        static const size_t rhs[0x005c];  //!< rhs
      private:
        YACK_DISABLE_COPY_AND_ASSIGN(senso20);
    };
  }
}
#endif
